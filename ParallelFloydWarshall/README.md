# Parallel Floyd-Warshall Algorithm in C++

This project implements the **Floyd-Warshall algorithm** for computing shortest paths between all pairs of nodes in a graph. It includes both:

- A **sequential version**
- A **parallel version** using C++20 threads (`std::jthread`)

The project also contains a **CLI-based test harness** to benchmark both versions and output the resulting adjacency matrices.

---

## 🗂 Files Included

| File Name               | Description |
|------------------------|-------------|
| `ParallelFloydWarshall.cpp` | Core implementation of graph generation and Floyd-Warshall (sequential + parallel). |
| `TestFloydWarshall.cpp`     | Main testing script with CLI arguments and output writing. |
| `floyd.h`                   | Matrix class and declarations for graph utilities and algorithms. |
| `random_int.h`              | Lightweight random integer generator class. |
| `clock.h`                   | Simple timing utility used for benchmarking. |

---

## 🧱 Dependencies

- C++20 or newer (required for `std::jthread`)
- A C++ compiler such as `g++`, `clang++`, or MSVC
- No external libraries required

---

## 🧪 Compilation

### Compile the core implementation:

```bash
g++ -std=c++20 -pthread -o ParallelFloydWarshall ParallelFloydWarshall.cpp
```

### Compile the test runner:

```bash
g++ -std=c++20 -pthread -o TestFloydWarshall TestFloydWarshall.cpp
```

Make sure all `.h` files (`floyd.h`, `random_int.h`, `clock.h`) are in the same directory.

---

## 🚀 Running the Test

Run the test executable to generate a random graph, run both versions of the algorithm, and compare performance:

```bash
./TestFloydWarshall [n] [distance] [side]
```

- `n`        – Number of vertices (default: 6)
- `distance` – Max edge connection distance (default: 40)
- `side`     – Side length of square area (default: 100)

### Example:

```bash
./TestFloydWarshall 100 50 200
```

This will:

1. Generate 100 random 2D points in a 200×200 square
2. Connect points within 50 units of distance
3. Run both Floyd-Warshall versions
4. Output the results to:
   - `mat_init.txt` — initial adjacency matrix
   - `mat_seq.txt` — after sequential algorithm
   - `mat_par.txt` — after parallel algorithm

---

## 📈 Performance Notes

- The parallel version assigns chunks of rows to available hardware threads.
- Threads synchronize at each iteration `k` of the Floyd-Warshall algorithm.
- The performance benefit becomes noticeable for graphs with 500+ vertices.

---

## ✅ Status

This project is complete and functions as expected. All results match between sequential and parallel versions.

---

## 🤝 Acknowledgments

- Uses standard Floyd-Warshall algorithm with row-wise parallelization.
- Built using modern C++20 features (`std::jthread`, `<chrono>`, `<random>`).
