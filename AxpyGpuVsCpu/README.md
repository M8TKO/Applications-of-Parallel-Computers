````markdown
# AXPY Benchmark (CUDA & C++ Threads)

This project implements the classic AXPY operation  
**c = α·a + b**  
on large vectors, and compares:

- A **GPU version** written in CUDA  
- A **CPU version** using C++20 threads

---

## 🗂 Files Included

| File                   | Description                                                             |
|------------------------|-------------------------------------------------------------------------|
| `AxpyBenchmark.cu`     | Single‐file CUDA + host C++ program. Contains:                          |
|                        | • `axpyKernel` CUDA kernel (stride‐based loop)                          |
|                        | • `axpy_cpu()` multi‐threaded CPU implementation                        |
|                        | • setup, timing, correctness check, and file logging.                   |
| `REZULTS.txt`          | Append‐only log of each run’s configuration and timings.                |

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
````

---

## 🚀 Running the Benchmark

```bash
./AxpyBenchmark
```

* Appends a new entry to `REZULTS.txt` with:

  * Grid size (# of blocks)
  * Block size (# of threads)
  * GPU execution time (µs)
  * CPU execution time (µs)
  * Maximum absolute error

---

## ✅ Status

✅ Complete and verified: GPU and CPU results match (max error ≈ 0).

---

## 🤝 Acknowledgments

* Demonstrates core parallel programming models: CUDA and C++ threading
* Uses modern C++ features for safety and simplicity (`std::vector`, RAII, `<chrono>`)

```
```
