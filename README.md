# cs692
Clone this repo and do cd cs692/
Clone and build google/benchmark 
Clone and build zstd

g++ zstd-benchmark.cpp -std=c++11 -I./zstd/lib -I./zstd/ -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\include" -isystem benchmark/include -Lbenchmark/build/src -L./zstd/lib -lzstd -lbenchmark -lpthread -o mybenchmark

Replace "C:\..." with the CUDA installation address
