#!/bin/bash

rm valsForExcel.txt
for numThreads in 1 2 4 6 8
do
	g++-7 -DNUMT=$numThreads -O3 -lm -fopenmp source.cpp -o prog1
	./prog1 >> valsForExcel.txt
done