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
- Built-in test runner with CLI support

---

## 🧱 Dependencies

- C++20 or newer (for `std::jthread`)
- Custom headers:
  - `floyd.h` — defines `Matrix` class and types
  - `random_int.h` — random integer generator
  - `clock.h` — simple timing utility

> Make sure these headers are in your project directory.

---

## 🧪 Compilation

To compile the core algorithm:

```bash
g++ -std=c++20 -pthread -o ParallelFloydWarshall ParallelFloydWarshall.cpp
```

To compile the test runner:

```bash
g++ -std=c++20 -pthread -o TestFloydWarshall TestFloydWarshall.cpp
```

---

## 🚀 Running the Test

Use the test runner to generate a random graph and benchmark both versions of the algorithm:

```bash
./TestFloydWarshall [n] [distance] [side]
```

- `n`        – Number of cities (vertices) to generate (default: 6)
- `distance` – Maximum distance to connect cities (default: 40)
- `side`     – Side length of the square area (default: 100)

### Example:

```bash
./TestFloydWarshall 100 50 200
```

This runs both the sequential and parallel versions, and writes matrices to:

- `mat_init.txt` — initial adjacency matrix
- `mat_seq.txt` — result after sequential Floyd-Warshall
- `mat_par.txt` — result after parallel Floyd-Warshall

---

## 📈 Performance Notes

- The parallel version runs `n` iterations (for each `k`) and divides the matrix rows among hardware threads.
- The benefit is noticeable for larger graphs (e.g., >500 nodes).
- Threads are safely joined on each outer iteration to synchronize row updates.

---

## ✅ Status

This project is complete and working as intended. All logic has been tested and validated.

---

## 🤝 Acknowledgments

- Based on the classical Floyd-Warshall algorithm
- Parallel logic adapted for row-wise matrix processing with C++20 threads
