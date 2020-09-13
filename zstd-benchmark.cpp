#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include "zstd.h"

using namespace std; 
vector<float> vec(1000);

static void BM_func(benchmark::State& state) {
    generate(vec.begin(), vec.end(), std::rand);
    
    for (auto _ : state) {
        size_t const compressed_buffer_size = ZSTD_compressBound(sizeof(vec));
        void *const compressed = malloc(compressed_buffer_size);
        size_t const cSize = ZSTD_compress(compressed,compressed_buffer_size,&vec,sizeof(vec),1);
    }
        
}

BENCHMARK(BM_func);

BENCHMARK_MAIN();