# AXPY Benchmark (CUDA & C++ Threads)

This project implements the classic **AXPY** operation  
$$
  \mathbf{c} = \alpha\,\mathbf{a} + \mathbf{b}
$$
on large vectors, and compares:

- A **GPU version** written in CUDA  
- A **CPU version** using C++20 threads

---

## 🗂 Files Included

| File                   | Description                                                             |
|------------------------|-------------------------------------------------------------------------|
| `AxpyBenchmark.cu`     | Single‐source CUDA + host C++ program. Contains:                        |
|                        | • `axpyKernel` CUDA kernel (stride‐based loop)                         |
|                        | • `axpy_cpu()` multi‐threaded CPU implementation                        |
|                        | • setup, timing, correctness check, and file logging.                  |
| `REZULTS.txt`          | Append‐only log of each run’s parameters and timings.                   |

---

## 🔐 Features

- **GPU kernel** with configurable grid/block and stride looping  
- **CPU multi-threading** via `std::thread` and dynamic chunking  
- **High‐resolution timing** using `<chrono>` (microsecond precision)  
- **Validation**: element‐wise max‐error comparison between GPU & CPU  
- **Automatic logging** of block/grid sizes, GPU & CPU times, and error

---

## 🧱 Dependencies

- **CUDA Toolkit** (nvcc, CUDA Runtime)  
- **C++20** compiler support (for `<thread>`, `<chrono>`, digit separators)  
- Standard library only—no third‐party libraries required

---

## 🧪 Compilation

```bash
nvcc -std=c++20 -o AxpyBenchmark AxpyBenchmark.cu
