#include "cascaded.hpp"
#include "nvcomp.hpp"
#include <iostream>
#include <chrono>
#include <cuda_runtime_api.h>
#include <cuda.h>

using namespace std; 
typedef std::chrono::high_resolution_clock Clock;

int main() {
	// Run a benchmark on the compress function of nvcc
	size_t size = sizeof(int)*409600000;
	int * uncompressed_data;
	cudaMalloc(reinterpret_cast<void**>(&uncompressed_data),sizeof(int)*409600000);
	
	 auto t1 = Clock::now();

	int * cpu_data = (int *)malloc(sizeof(int)*409600000);
	cudaMemcpy((void*)cpu_data, uncompressed_data, 409600000, cudaMemcpyDeviceToHost);
	
	cudaMemcpy((void*)uncompressed_data, cpu_data, 409600000, cudaMemcpyHostToDevice);
	
	 auto t2 = Clock::now();
	std::cout << "Copy done in " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " ns"  << std::endl;
	cudaFree(uncompressed_data);
	free(cpu_data);
	int uncompressed_count = 409600000;

	nvcomp::CascadedCompressor<int> compressor(
    	uncompressed_data, uncompressed_count, 2, 1, true);
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
