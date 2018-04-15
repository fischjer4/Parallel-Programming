#!/bin/bash

for numThreads in 1 2 4 6
do
	for numNodes in 500 1000 3000 5000 7000 10000 15000 20000
	do
		g++-7 -DNUMT=$numThreads -DNUMNODES=$numNodes -O3 -lm -fopenmp source.cpp -o prog1
		./prog1 >> valsForExcel.txt
	done
done