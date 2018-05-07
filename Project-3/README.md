## Pre-reqs
- XlsxWriter (install via _pip3 install XlsxWriter_)

## Instructions
1. Uncomment the lines that say _Uncomment for Fix #1_ in source.cpp
2. Uncomment the lines that say _Uncomment for Fix #1_ in runScript.sh
3. _sh runScript.sh_ (this generates valsForExcel.txt, which holds the values in order: \<number of threads\>,\<number of pads\>,\<time in microseconds\>)
4. Comment out the lines that say _Uncomment for Fix #1_, and uncomment the lines that say _Uncomment for Fix #2_ in source.cpp
5. Comment out the lines that say _Uncomment for Fix #1_, and uncomment the lines that say _Uncomment for Fix #2_ in source.cpp
1. Comment out the lines that say _Uncomment for Fix #1_, and uncomment the lines that say _Uncomment for Fix #2_ in runScript.sh
6. _sh runScript.sh_ (this generates fix2.txt, which holds the values in order: \<number of threads\>,\<time in microseconds\>)
3. _python3 generateExcelData.py_. This builds the excel file
4. _open results.xlsx_