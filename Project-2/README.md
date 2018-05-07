## Pre-reqs
- XlsxWriter (install via _pip3 install XlsxWriter_)

## Instructions
1. Make sure in source.cpp, the only output is _printf("%d,%d,%8.2lf \n", NUMT,NUMNODES,megaHeights);_
2. _sh runScript.sh_ (this generates valsForExcel.txt, which holds the values in order: \<threads\>,\<nodes\>,\<megaFunc\>)
3. _python3 generateExcelData.py_
4. _open results.xlsx_