#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>

// Password generation configuration
int num_passwords = 1'000'000;
int password_length = 12;

std::string generate_password(std::mt19937& rng, std::uniform_int_distribution<int>& dist) {
    static const std::string charset =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789_-";
    std::string password;
    password.reserve(password_length);
    for (int i = 0; i < password_length; ++i) {
        password += charset[dist(rng)];
    }
    return password;
}

// Parallel search configuration
int num_threads = 4;
std::atomic<bool> found = false;
std::mutex mut;

template <typename InputIt, typename UnaryPred>
void thread_find(InputIt first, InputIt last, UnaryPred predicate, std::promise<InputIt>& prom) {
    try {
        for (InputIt it = first; it != last; ++it) {
            if (found)
                return;

            if (predicate(*it)) {
                {
                    std::lock_guard lock(mut);
                    found = true;
                }
                prom.set_value(it);
                return;
            }
        }
    } catch (...) {
        std::lock_guard lock(mut);
        if (found)
            return;
        found = true;
        prom.set_exception(std::current_exception());
    }
}

template <typename InputIt, typename UnaryPred>
InputIt parfind_if(InputIt first, InputIt last, UnaryPred predicate) {
    std::promise<InputIt> promise;
    std::future<InputIt> future = promise.get_future();

    std::vector<std::jthread> threads;
    size_t chunk_size = std::distance(first, last) / num_threads;

    auto it = first;
    for (int i = 0; i < num_threads; ++i) {
        auto start = it;
        std::advance(it, chunk_size);
        auto end = (i == num_threads - 1) ? last : it;
        threads.emplace_back(thread_find<InputIt, UnaryPred>, start, end, predicate, std::ref(promise));
    }

    for (auto& t : threads)
        t.join();

    InputIt result = last;
    try {
        result = (found) ? future.get() : last;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    } catch (...) {
        std::cout << "Unknown exception occurred.\n";
    }

    return result;
}

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 63);

    std::vector<std::string> passwords;
    passwords.reserve(num_passwords);
    for (int i = 0; i < num_passwords; ++i) {
        passwords.push_back(generate_password(rng, dist));
    }

    std::string target_password = passwords[rng() % num_passwords];

    // Sequential search using std::find_if
    auto start_serial = std::chrono::high_resolution_clock::now();
    auto serial_result = std::find_if(passwords.begin(), passwords.end(), [&](const std::string& s) {
        return s == target_password;
    });
    auto end_serial = std::chrono::high_resolution_clock::now();
    auto serial_time = std::chrono::duration_cast<std::chrono::microseconds>(end_serial - start_serial);

    // Parallel search using parfind_if
    found = false;
    auto start_parallel = std::chrono::high_resolution_clock::now();
    auto parallel_result = parfind_if(passwords.begin(), passwords.end(), [&](const std::string& s) {
        return s == target_password;
    });
    auto end_parallel = std::chrono::high_resolution_clock::now();
    auto parallel_time = std::chrono::duration_cast<std::chrono::microseconds>(end_parallel - start_parallel);

    std::cout << "Target password: " << target_password << '\n';
    std::cout << "Sequential search time: " << serial_time.count() << " µs\n";
    std::cout << "Parallel search time:   " << parallel_time.count() << " µs\n";
    std::cout << "Sequential result: " << (serial_result != passwords.end() ? *serial_result : "Not found") << '\n';
    std::cout << "Parallel result:   " << (parallel_result != passwords.end() ? *parallel_result : "Not found") << '\n';

    return 0;
}
