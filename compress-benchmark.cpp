#include "lz4.hpp"
#include "nvcomp.hpp"
#include <iostream>
#include <chrono>
#include <cuda_runtime_api.h>
#include <cuda.h>

using namespace std; 
typedef std::chrono::high_resolution_clock Clock;
int size = 0;

void benchmark_assert(const bool pass, const std::string& msg)
{
  if (!pass) {
    throw std::runtime_error("ERROR: " + msg);
  }
}

int main(int argc, char *argv[]) {
	if(argc < 2)
		return -1; 
	
	// get the size of the array from command line. 
	std::string size_string = argv[1];

	size = std::stoi(size_string);
	
	cout << "Size is: " << size << endl;
	float * uncompressed_data;
	cout << "Allocating data" << endl;
	cudaMalloc(reinterpret_cast<void**>(&uncompressed_data),sizeof(float)*size);
	

	float * cpu_data = (float *)malloc(sizeof(float)*size);
	
	cout << "Making data" << endl;	
	
	for(int i = 0; i < size; i++) 
		cpu_data[i] = rand()*0.0195;	
	
	cout << "Starting copy" << endl;
	 auto t1 = Clock::now();
	// Not async no synchronize needed.
	cudaMemcpy((void*)uncompressed_data, cpu_data, size, cudaMemcpyHostToDevice);
		
	cudaMemcpy((void*)cpu_data, uncompressed_data, size, cudaMemcpyDeviceToHost);
	
	 auto t2 = Clock::now();
	
	std::cout << "Copy done in " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " ns"  << std::endl;
	cudaFree(uncompressed_data);
	free(cpu_data);
	
	// Benchmark for compression and decompression.
	float * data;	
	cudaMalloc(reinterpret_cast<void**>(&data), sizeof(float)*size);
	
	float * data1 = (float *) malloc(sizeof(float)*size);
	
	for(int i = 0; i < size; i++) 
		data1[i] = rand()*0.0195;
	
	cudaMemcpy((void*)data, data1, size, cudaMemcpyHostToDevice);
	
	cout << "input size: " << sizeof(float)*size << endl;
	
	
	// We can reinterpret cast here because LZ4 works on a byte level. 
        nvcomp::LZ4Compressor<uint8_t> compressor(
        reinterpret_cast<uint8_t*>(data), size*sizeof(float), 1 << 16);
        const size_t temp_size = compressor.get_temp_size();

        
	// Allocate temporary space here.
	void * temp_space;
        cudaMalloc(&temp_space, temp_size);

        size_t output_size = compressor.get_max_output_size(
    temp_space, temp_size);


        void * output_space;
        cudaMalloc(&output_space, output_size);
	

        cudaStream_t stream;
	 //  cudaStreamCreate(&stream);

	cudaError_t cudaErr = cudaStreamCreate(&stream);
        nvcompError_t status;

	auto t3 = Clock::now(); 
	
	compressor.compress_async(temp_space,
    temp_size, output_space, &output_size, stream);
	
	benchmark_assert(
      status == nvcompSuccess, "nvcompCascadedCompressAsync not successful");
 	cudaStreamSynchronize(stream);
	
	// nvcomp::LZ4Decompressor<uint8_t> decompressor(output,size,stream); 
	
	auto t4 = Clock::now();
	
	benchmark_assert(
      status == nvcompSuccess, "nvcompCascadedCompressAsync not successful");
 	cudaStreamSynchronize(stream);
	
	cout << "Time taken to compress and decompress is: " << chrono::duration_cast<chrono::nanoseconds>(t4 - t3).count() << " ns"  << std::endl;;
	cout << "Output size is: " << output_size << endl;
	
	cudaFree(temp_space);
	cudaFree(output_space);
	free(data1);	

}
