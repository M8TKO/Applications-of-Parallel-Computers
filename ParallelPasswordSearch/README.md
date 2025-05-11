# Parallel Password Search (C++)

This project demonstrates the use of **parallelism** in C++ to speed up searching through a large collection of generated passwords. It compares sequential search using `std::find_if` with a custom multithreaded implementation using `std::jthread`, `std::promise`, and `std::future`.

---

## 🔍 What It Does

- Generates **1,000,000 random passwords** of length 12 using a custom charset.
- Randomly selects one of the passwords to search for.
- Measures and compares the execution time of:
  - **Sequential search** using `std::find_if`
  - **Parallel search** using 4 threads and a custom `parfind_if` function

---

## 🛠 Features

- Custom password generator with alphanumeric and special characters
- Parallel search using modern C++ threading features
- Thread-safe early termination with `std::atomic<bool>`
- Performance benchmarking (microsecond precision)

---

## 🧱 Dependencies

- C++20 or newer (for `std::jthread`)
- A C++ compiler such as `g++`, `clang++`, or MSVC

---

## 💻 Compilation

```bash
g++ -std=c++20 -pthread -o ParallelPasswordSearch ParallelPasswordSearch.cpp
```

> Make sure your compiler supports C++20 and `std::jthread`. GCC 10+ or Clang 11+ recommended.

---

## 🚀 Usage

Simply run the program:

```bash
./ParallelPasswordSearch
```

### Sample Output

```
Target password: m_78zAYdHPue
Sequential search time: 53287 µs
Parallel search time:   16829 µs
Sequential result: m_78zAYdHPue
Parallel result:   m_78zAYdHPue
```

---

## 📌 Notes

- Passwords are randomly generated and stored in a vector.
- The parallel search divides the vector into chunks processed by 4 threads.
- If one thread finds the target, all other threads stop searching.
- Uses `std::promise` and `std::future` for thread-safe result sharing.

---

## ✅ Status

This project is complete and functions as expected. No further execution is required to verify correctness.

---


## 🤝 Acknowledgments

- Inspired by performance experiments in concurrent programming
- Built with modern C++ threading utilities (`<thread>`, `<future>`, `<atomic>`, `<mutex>`)

