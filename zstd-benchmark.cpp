#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include "zstd.h"

using namespace std; 
float * arr;

// __global__ some_kernel() {

// }

static void BM_cuda(benchmark::State& state) {
	for (auto _ : state) {
	int size = sizeof(float)*4096;
	arr = (float *)cudaMalloc(size);

        size_t const compressed_buffer_size = ZSTD_compressBound(size);
        // std::cout << "Original size is " << size << " " << "New size is " << compressed_buffer_size << endl;
        void *const compressed = cudaMalloc(compressed_buffer_size);
        size_t const cSize = ZSTD_compress(compressed,compressed_buffer_size,(void*)arr,size,1);
        cout << "Actual compressed size is: " << cSize << endl;
        size_t const dSize = ZSTD_decompress((void *)arr, size, compressed ,cSize);
        cout << "Decompressed size is: " << dSize << endl;
        cudaFree(arr);
        cudaFree(compressed);

	}
}

static void BM_func(benchmark::State& state) {
    
    for (auto _ : state) {
	 int size = sizeof(float)*4096;
    arr = (float *)malloc(size);

        size_t const compressed_buffer_size = ZSTD_compressBound(size);
	// std::cout << "Original size is " << size << " " << "New size is " << compressed_buffer_size << endl;
        void *const compressed = malloc(compressed_buffer_size);
        size_t const cSize = ZSTD_compress(compressed,compressed_buffer_size,(void*)arr,size,1);
	cout << "Actual compressed size is: " << cSize << endl;
       	size_t const dSize = ZSTD_decompress((void *)arr, size, compressed ,cSize);
	cout << "Decompressed size is: " << dSize << endl;
	free(arr);
	free(compressed);
    }
        
}

BENCHMARK(BM_func);
BENCHMARK(BM_cuda);

BENCHMARK_MAIN();
