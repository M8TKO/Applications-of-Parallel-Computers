# Parallel Floyd-Warshall Algorithm (C++)

This project implements the **Floyd-Warshall algorithm** for computing the shortest paths between all pairs of vertices in a graph. It includes both:

- A **sequential version**
- A **parallel version** using multiple threads with `std::jthread`

---

## 📌 Overview

- Vertices are randomly generated in a 2D space.
- Edges are created between vertices that lie within a given Euclidean distance.
- The edge weight is the rounded Euclidean distance between vertices.
- Parallelism is introduced by dividing the matrix row-wise per thread.

---

## 🛠 Features

- Custom 2D graph generator
- Distance-based edge creation
- All-pairs shortest path (Floyd-Warshall)
- Parallel matrix computation using threads
- Matrix pretty-printing with "inf" for unreachable vertices

---

## 🧱 Dependencies

- C++20 or newer (for `std::jthread`)
- Custom headers:
  - `floyd.h` — defines `Matrix` class and types
  - `random_int.h` — random integer generator

> Make sure these headers are in your project directory.

---

## 🧪 Compilation

```bash
g++ -std=c++20 -pthread -o ParallelFloydWarshall ParallelFloydWarshall.cpp
```

---

## 🚀 Usage

You can call the `generate_vertices`, `generate_edges`, `minimum_distance`, and `minimum_distance_par` functions from your main function depending on whether you want sequential or parallel execution.

### Example

```cpp
Vertices points;
Matrix M;

generate_vertices(100, points, 500);          // 100 vertices within a 500×500 grid
generate_edges(100.0, points, M);             // Connect vertices within 100 units
minimum_distance_par(M);                      // Compute all-pairs shortest paths in parallel
print(std::cout, M);                          // Output the final distance matrix
```

---

## 📈 Performance Notes

- The parallel version runs `n` iterations (for each `k`) and divides the matrix rows among hardware threads.
- The benefit is noticeable for larger graphs (e.g., >500 nodes).
- Threads are safely joined on each outer iteration to synchronize row updates.

---

## ✅ Status

This project is complete and working as intended. All logic has been tested and validated. No runtime validation is necessary unless changes are made.

---

## 🤝 Acknowledgments

- Based on the classical Floyd-Warshall algorithm
- Parallel logic adapted for row-wise matrix processing with C++20 threads
