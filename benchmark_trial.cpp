#include "../zfp/include/zfp.h"
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <cuda_runtime_api.h>
#include <cuda.h>

using namespace std; 
typedef std::chrono::high_resolution_clock Clock;

int size; 

int main(int argc, char* argv[]) {
	if(argc < 2)
		return -1; 
	size = atoi(argv[1]);
	
	void * array;
	cudaMalloc(&array,sizeof(double) * size);
	std::cout << "Input size" << sizeof(double) * size << std::endl;
	// initialize metadata for the 3D array a[nz][ny][nx]
	zfp_type type = zfp_type_double;                          // array scalar type
	zfp_field* field = zfp_field_1d(array, type, size); // array metadata

	zfp_stream* zfp = zfp_stream_open(NULL);                  // compressed stream and parameters
	zfp->maxbits = 16; 	
	// initialize metadata for a compressed stream
	zfp_stream_set_rate(zfp, 12, type, 1, 0);


	// zfp_stream_set_accuracy(zfp, tolerance);                  // set tolerance for fixed-accuracy mode
	//  zfp_stream_set_precision(zfp, precision);             // alternative: fixed-precision mode
	//  zfp_stream_set_rate(zfp, rate, type, 3, 0);           // alternative: fixed-rate mode

	// allocate buffer for compressed data
	size_t bufsize = zfp_stream_maximum_size(zfp, field);     // capacity of compressed buffer (conservative)
	void* buffer; 
	cudaMalloc(&buffer, bufsize);                           // storage for compressed stream

	// associate bit stream with allocated buffer
	bitstream* stream = stream_open(buffer, bufsize);         // bit stream to compress to
	zfp_stream_set_bit_stream(zfp, stream);                   // associate with compressed stream
	zfp_stream_rewind(zfp);                                   // rewind stream to beginning


	if (zfp_stream_set_execution(zfp, zfp_exec_cuda)) {
		auto t1 = Clock::now();
		size_t zfpsize = zfp_compress(zfp, field);                // return value is byte size of compressed stream
		auto t2 = Clock::now();
		std::cout << zfpsize << " Final output size" << std::endl;
		std::cout << "Time taken to compress and decompress is: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " ns"  << std::endl;
	} else {
		std::cout << "Nope" << std::endl;
	}
}
