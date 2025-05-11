#include "ts_map.h"
#include <string>
#include <cassert>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void insert_range(Map<std::string, int>& map, int start, int end, int& cnt) {
    cnt = 0;
    for (int i = start; i < end; ++i) {
        std::string key = "K" + std::to_string(i);
        if (map.insert(key, i)) ++cnt;
        std::this_thread::sleep_for(1ms);
    }

    if (start == 0)
        map.print("first_insert.txt");
    else
        map.print("second_insert.txt");
}

void read_range(Map<std::string, int> const& map, int start, int end, int& cnt_found) {
    cnt_found = 0;
    for (int i = start; i < end; ++i) {
        std::string key = "K" + std::to_string(i);
        if (map.get(key).has_value()) ++cnt_found;
        std::this_thread::sleep_for(1ms);
    }
}

void fun_assign(Map<std::string, int>& map, int start, int end, int& cnt) {
    cnt = 0;
    std::this_thread::sleep_for(10ms);
    for (int i = start; i < end; ++i) {
        std::string key = "K" + std::to_string(i);
        if (map.assign(key, 1000 + i)) ++cnt;
        std::this_thread::sleep_for(1ms);
    }
    map.print("fun_assign.txt");
}

void remove_range(Map<std::string, int>& map, int start, int end, int& cnt_removed) {
    cnt_removed = 0;
    for (int i = start; i < end; ++i) {
        std::string key = "K" + std::to_string(i);
        if (map.remove(key)) ++cnt_removed;
        std::this_thread::sleep_for(1ms);
    }
    map.print("after_remove.txt");
}

int main() {
    Map<std::string, int> map;

    int inserted1 = 0, inserted2 = 0;
    int read_before_remove = 0;
    int removed = 0;
    int assigned = 0;

    std::thread t1(insert_range, std::ref(map), 0, 100, std::ref(inserted1));
    std::thread t2(insert_range, std::ref(map), 50, 150, std::ref(inserted2));

    std::this_thread::sleep_for(10ms);
    std::thread t3(read_range, std::cref(map), 0, 150, std::ref(read_before_remove));

    std::this_thread::sleep_for(10ms);
    std::thread t4(remove_range, std::ref(map), 30, 120, std::ref(removed));
    std::thread t5(fun_assign, std::ref(map), 30, 120, std::ref(assigned));

    t1.join(); t2.join(); t3.join(); t4.join(); t5.join();

    int final_size = map.size();

    std::cout << "Inserted (t1): " << inserted1 << std::endl;
    std::cout << "Inserted (t2): " << inserted2 << std::endl;
    std::cout << "Found during read: " << read_before_remove << std::endl;
    std::cout << "Removed: " << removed << std::endl;
    std::cout << "Modified: " << assigned << std::endl;
    std::cout << "Final map size: " << final_size << std::endl;

    map.print("final_state.txt");

    assert(inserted1 + inserted2 >= final_size + removed);
    std::cout << "Test passed successfully.\n";

    return 0;
}
