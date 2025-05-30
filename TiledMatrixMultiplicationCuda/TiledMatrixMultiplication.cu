#include <iostream>
#include <random>
#include <cmath>    // for std::fabs
#include <cuda_runtime.h>

#define TILE_WIDTH 23

/**
 * CUDA kernel for tiled matrix multiplication:
 *   C = A × B
 * Matrices are square (N × N), stored row-major.
 * Handles cases where N is not a multiple of TILE_WIDTH by
 * padding out‐of‐bounds loads with zero.
 */
__global__
void matMulKernel(const float* A, const float* B, float* C, int N) {
    // Thread and block indices
    int tx = threadIdx.x, ty = threadIdx.y;
    int bx = blockIdx.x,  by = blockIdx.y;
    // Global row and column this thread computes
    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;

    // Shared tile buffers
    __shared__ float tileA[TILE_WIDTH][TILE_WIDTH];
    __shared__ float tileB[TILE_WIDTH][TILE_WIDTH];

    float sum = 0.0f;
    int numTiles = (N + TILE_WIDTH - 1) / TILE_WIDTH;

    // Loop over all tile phases
    for (int t = 0; t < numTiles; ++t) {
        int aCol = t * TILE_WIDTH + tx;
        int bRow = t * TILE_WIDTH + ty;

        // Load A[row, aCol] into shared memory, or 0 if out of bounds
        if (row < N && aCol < N) {
            tileA[ty][tx] = A[row * N + aCol];
        } else {
            tileA[ty][tx] = 0.0f;
        }
        // Load B[bRow, col] into shared memory, or 0 if out of bounds
        if (bRow < N && col < N) {
            tileB[ty][tx] = B[bRow * N + col];
        } else {
            tileB[ty][tx] = 0.0f;
        }

        __syncthreads();

        // Multiply the two tiles
        int effectiveWidth = min(TILE_WIDTH, N - t * TILE_WIDTH);
        if (row < N && col < N) {
            for (int k = 0; k < effectiveWidth; ++k) {
                sum += tileA[ty][k] * tileB[k][tx];
            }
        }

        __syncthreads();
    }

    // Write result if within bounds
    if (row < N && col < N) {
        C[row * N + col] = sum;
    }
}

/**
 * Check CUDA API errors and exit on failure.
 */
inline void checkCuda(cudaError_t status, const char* file, int line) {
    if (status != cudaSuccess) {
        std::cerr << "CUDA error: " << cudaGetErrorString(status)
                  << " at " << file << ":" << line << "\n";
        std::exit(EXIT_FAILURE);
    }
}
#define CUDA_CHECK(call) checkCuda((call), __FILE__, __LINE__)

/**
 * Host‐side reference matrix multiplication and result checker.
 * Returns true if max absolute error ≤ EPS × max value.
 */
bool checkResult(const float* A, const float* B, const float* C, int N) {
    const float EPS = 1e-6f;
    float maxError = 0.0f, maxValue = 0.0f;

    // Compute reference C_ref = A × B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float tmp = 0.0f;
            for (int k = 0; k < N; ++k) {
                tmp += A[i * N + k] * B[k * N + j];
            }
            float diff = std::fabs(tmp - C[i * N + j]);
            float val  = std::fabs(tmp);
            maxError = std::max(maxError, diff);
            maxValue = std::max(maxValue, val);
        }
    }

    if (maxError > EPS * maxValue) {
        std::cout << "(Max error = " << maxError
                  << ", max value = " << maxValue
                  << ", rel. error = " << (maxError / maxValue) << ")\n";
        return false;
    }
    return true;
}

int main() {
    // Matrix dimension
    const int N = 1024;

    // Host allocations
    float* h_A = new float[N * N];
    float* h_B = new float[N * N];
    float* h_C = new float[N * N];

    // Random normal distribution generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.0f, 1.0f);

    // Initialize host matrices A and B
    for (int i = 0; i < N * N; ++i) {
        h_A[i] = dist(gen);
        h_B[i] = dist(gen);
    }

    // Device allocations
    float *d_A, *d_B, *d_C;
    CUDA_CHECK(cudaMalloc(&d_A, N * N * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_B, N * N * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_C, N * N * sizeof(float)));

    // Copy inputs to device
    CUDA_CHECK(cudaMemcpy(d_A, h_A, N * N * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_B, h_B, N * N * sizeof(float), cudaMemcpyHostToDevice));

    // Configure kernel launch
    dim3 blockDim(TILE_WIDTH, TILE_WIDTH);
    dim3 gridDim((N + TILE_WIDTH - 1) / TILE_WIDTH,
                 (N + TILE_WIDTH - 1) / TILE_WIDTH);

    // Create and record CUDA events for timing
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));
    CUDA_CHECK(cudaEventRecord(start));

    // Launch the tiled matrix multiplication kernel
    matMulKernel<<<gridDim, blockDim>>>(d_A, d_B, d_C, N);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaEventRecord(stop));
    CUDA_CHECK(cudaEventSynchronize(stop));

    // Measure elapsed time in milliseconds
    float milliseconds = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&milliseconds, start, stop));
    std::cout << "GPU time: " << milliseconds << " ms\n";

    // Copy result back to host
    CUDA_CHECK(cudaMemcpy(h_C, d_C, N * N * sizeof(float), cudaMemcpyDeviceToHost));

    // Validate result on host
    bool ok = checkResult(h_A, h_B, h_C, N);
    std::cout << "Result is " << (ok ? "correct.\n" : "incorrect!\n");

    // Cleanup
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;

    return 0;
}
