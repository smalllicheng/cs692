# cs692
Clone this repo and do cd cs692/
Clone and build google/benchmark 
Clone and build zstd

export LD_LIBRARY_PATH=/usr/local/cuda-11.0/lib64:$LD_LIBRARY_PATH
export PATH=/usr/local/cuda-11.0/bin:$PATH
g++ zstd-benchmark.cpp -std=c++11 -I /usr/local/cuda/ -I /usr/local/cuda/include -I./zstd/lib -I./zstd/ -isystem benchmark/include -Lbenchmark/build/src -L./zstd/lib -L/usr/local/cuda/lib64 -lcudart -lzstd  -lbenchmark -lpthread  -o mybenchmark

Clone and build Nvidia/nvcomp according to instructions the their repo
Also add cuda and nvcomp to LD preload path.
g++ -I /usr/local/cuda/ -I /usr/local/cuda/include -I./nvcomp/build/include compress-benchmark.cpp -o compress -lcudart -lnvcomp -L/usr/local/cuda/lib64 -L./nvcomp/build/lib 

Windows - 
g++ zstd-benchmark.cpp -std=c++11 -I./zstd/lib -I./zstd/ -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\include" -isystem benchmark/include -Lbenchmark/build/src -L./zstd/lib -lzstd -lbenchmark -lpthread -o mybenchmark

Replace "C:\..." with the CUDA installation address
