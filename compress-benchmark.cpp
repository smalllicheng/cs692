#include "lz4.hpp"
#include "nvcomp.hpp"
#include <iostream>
#include <chrono>
#include <cuda_runtime_api.h>
#include <cuda.h>

using namespace std; 
typedef std::chrono::high_resolution_clock Clock;


void benchmark_assert(const bool pass, const std::string& msg)
{
  if (!pass) {
    throw std::runtime_error("ERROR: " + msg);
  }
}

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
	
	float * data;	
	cudaMalloc(reinterpret_cast<void**>(&data), sizeof(float)*4096000);
	
	float * data1 = (float *) malloc(sizeof(float)*4096000);
	
	for(int i = 0; i < 4096000; i++) 
		data1[i] = rand();
	
	cudaMemcpy((void*)data, data1, 4096000, cudaMemcpyHostToDevice);
	
	cout << "input size: " << sizeof(float)*4096000 << endl;
	
        nvcomp::LZ4Compressor<uint8_t> compressor(
        reinterpret_cast<uint8_t*>(data), 4096000*sizeof(float), 1 << 16);
        const size_t temp_size = compressor.get_temp_size();

        void * temp_space;
        cudaMalloc(&temp_space, temp_size);

        size_t output_size = compressor.get_max_output_size(
    temp_space, temp_size);

	cout << "Output size is: " << output_size << endl; 

        void * output_space;
        cudaMalloc(&output_space, output_size);
	

        cudaStream_t stream;
	  cudaStreamCreate(&stream);

	cudaError_t cudaErr = cudaStreamCreate(&stream);
        nvcompError_t status;
	compressor.compress_async(temp_space,
    temp_size, output_space, &output_size, stream);

	
	benchmark_assert(
      status == nvcompSuccess, "nvcompCascadedCompressAsync not successful");
 	cudaStreamSynchronize(stream);

	cout << "Output size is: " << output_size << endl;

	cudaFree(temp_space);
	cudaFree(output_space);
	

}
