# cs692
Clone this repo and do cd cs692/
Clone and build google/benchmark 
Clone and build zstd

export LD_LIBRARY_PATH=/usr/local/cuda-11.0/lib64:$LD_LIBRARY_PATH
export PATH=/usr/local/cuda-11.0/bin:$PATH
g++ zstd-benchmark.cpp -std=c++11 -I /usr/local/cuda/ -I /usr/local/cuda/include -I./zstd/lib -I./zstd/ -isystem benchmark/include -Lbenchmark/build/src -L./zstd/lib -L/usr/local/cuda/lib64 -lcudart -lzstd  -lbenchmark -lpthread  -o mybenchmark

Clone and build Nvidia/nvcomp according to instructions the their repo
Also add cuda and nvcomp to LD preload path.
export LD_LIBRARY_PATH=/home/shauryapatel1995/nvcomp/build/lib:$LD_LIBRARY_PATH
g++ -I /usr/local/cuda/ -I /usr/local/cuda/include -I./nvcomp/build/include compress-benchmark.cpp -o compress -lcudart -lnvcomp -L/usr/local/cuda/lib64 -L./nvcomp/build/lib 

Windows - 
g++ zstd-benchmark.cpp -std=c++11 -I./zstd/lib -I./zstd/ -I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.0\include" -isystem benchmark/include -Lbenchmark/build/src -L./zstd/lib -lzstd -lbenchmark -lpthread -o mybenchmark

Replace "C:\..." with the CUDA installation address

zfp benchmarks - 
Clone zfp and use the cmake -DZFP_WITH_CUDA=ON .. to create vuild files with cuda compiler on. 
Use LD_LIBRARY_PATH to add libraries to it.
Compile g++ -std=c++11 -I/usr/local/cuda/ -I/usr/local/cuda/include benchmark_trial.cpp -o zfp_benchmark -lzfp -lcudart -L/usr/local/cuda/lib64 -L../zfp/build/lib
run ./zfp_benchmark 40960000




Processing ImageNet2012

1. Download the ImageNet2012 dataset from http://www.image-net.org/challenges/LSVRC/2012/downloads

2. Use the script to extract it recursively https://gist.github.com/BIGBALLON/8a71d225eff18d88e469e6ea9b39cef4

2.1. Comment line 34 in the script "wget -qO- https:...." which makes the format conflict with converting script

3. Get list file from caffe https://github.com/BVLC/caffe/blob/master/data/ilsvrc12/get_ilsvrc_aux.sh

4. Use the scripts SN provided, I choose to fill in the parameters instead of using args Here's my example for val.

    src_folder = "/home/jupyter/imagenet/val"    #sys.argv[1]
    
    list_file = "/home/jupyter/imagenet/val.txt"  #sys.argv[2]
    
    dst_folder = "/home/jupyter/inet" #sys.argv[3]
    
    prefix = "val" #sys.argv[4]
    
    height, width = 256,256 #int(sys.argv[5]), int(sys.argv[6])
    
    splits = 1 #int(sys.argv[7])
    
    shuffle = False #int(sys.argv[8]) == 1
    
    workers = 8 #int(sys.argv[9])
    
5. Compute the image mean
5.1 move mean.py, binary.py to the directory we created in step 4
5.2 fillin or use args as they mentioned in example
6. Wait
