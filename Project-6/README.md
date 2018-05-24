# OpenCL Array Multiply, Multiply-Add, and Multiply-Reduce

# Assignment
There are many problems in scientific computing where you want to do arithmetic on multiple arrays of numbers (matrix manipulation, Fourier transformation, convolution, etc.). This project is in two parts:
1. Multiply two arrays together using OpenCL: D[gid] = A[gid]\*B[gid]; Benchmark it against both input array size (i.e., the global work size) and the local work size (i.e., number of work-items per work-group).
2. Multiply two arrays together and add a third using OpenCL: D[gid] = A[gid]\*B[gid] + C[gid]; Benchmark it against both input array size (i.e., the global work size) and the local work size (i.e., number of work-items per work-group).
3. Perform the same array multiply as in #1, but this time with a reduction: Sum = summation{ A[:]\*B[:] }; Benchmark that against input array size (i.e., the global work size). You can pick a local work size and hold that constant.


## Pre-reqs
- XlsxWriter (install via _pip3 install XlsxWriter_)

## Instructions To Run
3. _sh runScript.sh_
3. _python3 generateExcelData.py_. This builds the excel file
4. _open results.xlsx_
