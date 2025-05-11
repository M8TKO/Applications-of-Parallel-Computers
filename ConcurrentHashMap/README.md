# Concurrent Map (Thread-Safe Hash Map in C++)

This project implements a **thread-safe associative container** (similar to `std::unordered_map`) using **bucketed locking** with `std::shared_mutex`. The map supports concurrent reads and writes, making it suitable for multi-threaded applications. It also includes a **stress test program** that exercises concurrent insertions, deletions, lookups, and updates.

---

## 🗂 Files Included

| File Name              | Description |
|------------------------|-------------|
| `ConcurrentMap.h`      | Thread-safe hash map implementation with shared locks per bucket |
| `test.cpp`| Multithreaded test that exercises all operations and verifies correctness |

---

## 🔐 Features

- **Insert**, **remove**, **update**, and **read** operations
- Uses `std::shared_mutex` for **reader-writer locking**
- Per-bucket locking for fine-grained concurrency
- Thread-safe size tracking
- Can print map contents to file for debugging

---

## 🧱 Dependencies

- C++17 or newer (for `std::shared_mutex`, `std::optional`, etc.)
- Standard libraries only — no external dependencies

---

## 🧪 Compilation

To compile the test program:

```bash
g++ -std=c++17 -pthread -o test test.cpp
```

> Ensure that `ConcurrentMap.h` is in the same directory.

---

## 🚀 Running the Test

```bash
./test
```

### What It Does:

1. Launches multiple threads that:
   - Insert overlapping ranges of keys
   - Read existing keys
   - Assign new values to some keys
   - Remove a subset of keys
2. Prints intermediate and final map contents to:
   - `first_insert.txt`
   - `second_insert.txt`
   - `fun_assign.txt`
   - `after_remove.txt`
   - `final_state.txt`
3. Verifies correctness with a final assertion on the expected size.

### Example Output:

```
Inserted (t1): 100
Inserted (t2): 50
Found during read: 120
Removed: 60
Modified: 30
Final map size: 90
Test passed successfully.
```

---

## ✅ Status

This project is complete and correctly handles concurrent operations on the custom map. No further verification is required unless logic is modified.

---

## 🤝 Acknowledgments

- Inspired by concurrent hash maps and bucketed-lock designs
- Built with standard C++17 synchronization primitives
