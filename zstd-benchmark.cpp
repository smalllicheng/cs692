#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include "zstd.h"

using namespace std;
float *arr;

// Kernel function to run the compression on the GPU.
__global__ static void some_kernel()
{
}

/*
*   Benchmark function responsible to run compression on GPU memory.
*/
static void BM_cuda(benchmark::State &state)
{
    for (auto _ : state) {
        int size = sizeof(float) * 4096;
        arr = (float *)cudaMalloc(size);
        size_t const compressed_buffer_size = ZSTD_compressBound(size);
        void *const compressed = cudaMalloc(compressed_buffer_size);
        size_t const cSize = ZSTD_compress(compressed, compressed_buffer_size, (void *)arr, size, 1);
        cout << "Actual compressed size is: " << cSize << endl;
        size_t const dSize = ZSTD_decompress((void *)arr, size, compressed, cSize);
        cout << "Decompressed size is: " << dSize << endl;
        cudaFree(arr);
        cudaFree(compressed);
    }
}

/*
*   Benchmark function to run benchmark on CPU memory.
*/
static void BM_func(benchmark::State &state)
{

    for (auto _ : state) {
        int size = sizeof(float) * 4096;
        arr = (float *)malloc(size);
        size_t const compressed_buffer_size = ZSTD_compressBound(size);
        void *const compressed = malloc(compressed_buffer_size);
        size_t const cSize = ZSTD_compress(compressed, compressed_buffer_size, (void *)arr, size, 1);
        cout << "Actual compressed size is: " << cSize << endl;
        size_t const dSize = ZSTD_decompress((void *)arr, size, compressed, cSize);
        cout << "Decompressed size is: " << dSize << endl;
        free(arr);
        free(compressed);
    }
}

// Registration of the benchmark functions.
BENCHMARK(BM_func);
BENCHMARK(BM_cuda);

// Run the benchmarks.
BENCHMARK_MAIN();
