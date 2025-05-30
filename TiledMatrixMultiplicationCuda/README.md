````markdown
# Tiled Matrix Multiplication (CUDA)

This project implements a **tiled** (blocked) matrix multiplication kernel in CUDA, computing `C = A * B` for square matrices of size N×N. It uses a **shared-memory tile algorithm** and correctly handles cases where N is not a multiple of the tile width.

---

## 🗂 Files Included

| File                           | Description                                                                   |
|--------------------------------|-------------------------------------------------------------------------------|
| `TiledMatrixMultiplication.cu` | CUDA source: tiled kernel, host allocation, data initialization, timing, and validation. |
| `README.md`                    | This overview.                                                                |

---

## 🔐 Features

- **Tiled multiplication** with compile-time `TILE_WIDTH`  
- **Shared memory** (`__shared__`) for coalesced loads and reduced global memory traffic  
- **Boundary handling**: out-of-bounds loads padded with zero when `N % TILE_WIDTH ≠ 0`  
- **CUDA error checking** macro (`CUDA_CHECK`) for robust API call validation  
- **Kernel timing** using `cudaEvent_t` (reports GPU execution time in ms)  
- **Result validation**: host-side reference multiplication and max-error check  

---

## 🧱 Dependencies

- **CUDA Toolkit** (nvcc, CUDA Runtime)  
- **C++** compiler with `<random>`, `<cmath>`, and C++20 support  
- Standard library only—no external dependencies  

---

## 🧪 Compilation

```bash
nvcc -std=c++20 -o TiledMatrixMultiplication TiledMatrixMultiplication.cu
````

---

## 🚀 Running

```bash
./TiledMatrixMultiplication
```

* Allocates two N×N matrices with random normal values
* Launches the tiled CUDA kernel
* Prints:

  * **GPU time** (milliseconds)
  * **Validation result** (correct/incorrect)

---

## ✅ Status

Complete and tested for N=1024. Kernel output matches host reference (max relative error ≤ 1e-6).

---

## 🤝 Acknowledgments

* Demonstrates CUDA shared-memory tiling strategy
* Illustrates boundary-safe kernel design and error checking
* Utilizes modern C++ features for clear initialization and validation

```
```
