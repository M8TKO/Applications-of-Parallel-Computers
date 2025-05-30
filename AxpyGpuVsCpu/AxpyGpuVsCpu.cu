#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;

// Initialize host array x of length dim with random floats in [0,1)
void random_init(float* x, int dim) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int i = 0; i < dim; ++i) 
        x[i] = dist(gen);
}

// GPU AXPY kernel: c[i] = alpha * a[i] + b[i]
__global__
void axpyKernel(const float* a, const float* b, float* c, float alpha, int N) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    // Stride loop for cases where grid*block < N
    while (i < N) {
        c[i] = alpha * a[i] + b[i];
        i += gridDim.x * blockDim.x;
    }
}

// Multithreaded CPU version of AXPY using std::thread
void axpy_cpu(const float* a, const float* b, float* c, float alpha, int N) {
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;  // Fallback if detection fails

    int chunk = (N + num_threads - 1) / num_threads;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // Launch threads, each handling a contiguous chunk
    for (unsigned int t = 0; t < num_threads; ++t) {
        int start = t * chunk;
        int end   = std::min(start + chunk, N);
        threads.emplace_back([=]() {
            for (int i = start; i < end; ++i) {
                c[i] = alpha * a[i] + b[i];
            }
        });
    }
    // Wait for all to finish
    for (auto& th : threads) {
        th.join();
    }
}

int main() {
    // Open results file (append mode)
    std::ofstream results("REZULTS.txt", std::ios::out | std::ios::app);
    if (!results) {
        std::cerr << "Failed to open RESULTS.txt!\n";
        return 1;
    }
    // Redirect std::cout to the results file
    auto old_buf = std::cout.rdbuf(results.rdbuf());

    int N = 10'000'000;
    // Allocate and initialize host arrays
    float* h_a     = new float[N]; random_init(h_a, N);
    float* h_b     = new float[N]; random_init(h_b, N);
    float* h_c_gpu = new float[N];
    float* h_c_cpu = new float[N];
    float alpha    = 2.0f;

    // Allocate device arrays
    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, N * sizeof(float));
    cudaMalloc(&d_b, N * sizeof(float));
    cudaMalloc(&d_c, N * sizeof(float));

    // Copy inputs to device
    cudaMemcpy(d_a, h_a, N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, N * sizeof(float), cudaMemcpyHostToDevice);

    // Launch configuration: 256 threads per block
    dim3 blockDim(256);
    dim3 gridDim((N + blockDim.x - 1) / blockDim.x);

    // Time the GPU kernel
    auto t0 = high_resolution_clock::now();
    axpyKernel<<<gridDim, blockDim>>>(d_a, d_b, d_c, alpha, N);
    cudaDeviceSynchronize();
    auto t1 = high_resolution_clock::now();
    long gpu_us = duration_cast<microseconds>(t1 - t0).count();

    // Copy result back
    cudaMemcpy(h_c_gpu, d_c, N * sizeof(float), cudaMemcpyDeviceToHost);

    // Time the CPU version
    t0 = high_resolution_clock::now();
    axpy_cpu(h_a, h_b, h_c_cpu, alpha, N);
    t1 = high_resolution_clock::now();
    long cpu_us = duration_cast<microseconds>(t1 - t0).count();

    // Verify correctness
    float max_error = 0.0f;
    for (int i = 0; i < N; ++i) {
        max_error = std::max(max_error, std::fabs(h_c_gpu[i] - h_c_cpu[i]));
    }

    // Output benchmark and error
    std::cout << "AXPY on " << N << " elements (alpha=" << alpha << ")\n";
    std::cout << "\tGrid size (blocks)     = " << gridDim.x << "\n";
    std::cout << "\tBlock size (threads)   = " << blockDim.x << "\n";
    std::cout << "\tGPU time               = " << gpu_us << " µs\n";
    std::cout << "\tCPU time               = " << cpu_us << " µs\n";
    std::cout << "\tMaximum error          = " << max_error << "\n\n";

    // Cleanup
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    delete[] h_a;
    delete[] h_b;
    delete[] h_c_gpu;
    delete[] h_c_cpu;

    // Restore std::cout
    std::cout.rdbuf(old_buf);
    return 0;
}
