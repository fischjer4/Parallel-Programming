#!/bin/bash

# Array Multiplication and Array Multiplication and Add
rm arrayMult.txt
rm arrayMultAdd.txt
for locSize in 8 16 32 64 128 256 512
do
	for numEle in 1024 4096 8192 32768 65536 131072 262144 524288 1048576 2097152 3145728 4194304 5242880 6291456 7340032 8388608
	do
		g++ -o progMult source.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -DNUM_ELEMENTS=$numEle -DLOCAL_SIZE=$locSize
		./progMult >> arrayMult.txt

		g++ -o progMultAdd source.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -DMULT_ADD -DNUM_ELEMENTS=$numEle -DLOCAL_SIZE=$locSize
		./progMultAdd >> arrayMultAdd.txt
	done
done

# Array Multiplication With Reduction
rm arrayMultReduction.txt
for numEle in 1024 4096 8192 32768 65536 131072 262144 524288 1048576 2097152 3145728 4194304 5242880 6291456 7340032 8388608
do
	g++ -o progReduction sourceReduction.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp -DNUM_ELEMENTS=$numEle -DLOCAL_SIZE=32
	./progReduction >> arrayMultReduction.txt
done
