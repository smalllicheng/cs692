#include "lz4.hpp"
#include "nvcomp.hpp"
#include <iostream>
#include <chrono>
#include <cuda_runtime_api.h>
#include <cuda.h>

using namespace std; 
typedef std::chrono::high_resolution_clock Clock;

int main() {
	// Run a benchmark on the compress function of nvcc
	size_t size = sizeof(int)*4096;
	int * uncompressed_data;
	cudaMalloc(reinterpret_cast<void**>(&uncompressed_data),sizeof(int)*4096);
	
	 auto t1 = Clock::now();

	int * cpu_data = (int *)malloc(sizeof(int)*4096);
	cudaMemcpy((void*)cpu_data, uncompressed_data, 4096, cudaMemcpyDeviceToHost);
	
	cudaMemcpy((void*)uncompressed_data, cpu_data, 4096, cudaMemcpyHostToDevice);
	
	 auto t2 = Clock::now();
	std::cout << "Copy done in " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " ns"  << std::endl;
	cudaFree(uncompressed_data);
	free(cpu_data);
	

        nvcomp::LZ4Compressor<uint32_t> compressor(
        (unsigned int *)uncompressed_data, 4096, 65536);
        const size_t temp_size = compressor.get_temp_size();

        void * temp_space;
        cudaMalloc(&temp_space, temp_size);

        size_t output_size = compressor.get_max_output_size(
    temp_space, temp_size);

        void * output_space;
        cudaMalloc(&output_space, output_size);


        cudaStream_t stream;

cudaError_t cudaErr = cudaStreamCreate(&stream);
        nvcompError_t status;
compressor.compress_async(temp_space,
    temp_size, output_space, &output_size, stream);


	std::cout << "Done" << std::endl;

}
