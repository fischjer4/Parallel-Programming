#!/bin/bash

# Used for Fix #1: The Padding solution
rm valsForExcel.txt
for numThreads in 1 2 4 6
do
	for NUMPAD in {0..15}
	do
		g++-7 -DNUMT=$numThreads -DNUMPAD=$NUMPAD -O0 -lm -fopenmp source.cpp -o prog1
		./prog1 >> valsForExcel.txt
	done
done


# Used for Fix #2: The private variable solution
# rm fix2.txt
# for numThreads in 1 2 4 6
# do
# 	g++-7 -DNUMT=$numThreads -O0 -lm -fopenmp source.cpp -o prog1
# 	./prog1 >> fix2.txt
# done