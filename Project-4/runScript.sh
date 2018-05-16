#!/bin/bash

rm valsForExcel.txt
g++-7 -O3 -lm -fopenmp source.cpp -o prog
./prog >> valsForExcel.txt
