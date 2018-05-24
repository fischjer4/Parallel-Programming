#!/bin/csh

### Uncomment For NO Vectorization ####
rm noSIMDvalues.txt
foreach numThreads (1 2 4 6 8 16)
	foreach NUMS (1000 10000 100000 1000000 2000000 15000000 30000000 60000000)
		icpc -o prog source.cpp -lm -DNUMT=$numThreads -DNUMS=$NUMS -openmp -align -qopt-report=3 -qopt-report-phase=vec -no-vec
		./prog >>! noSIMDvalues.txt
	end
end
mv source.optrpt noSIMD.optrpt

### Uncomment FOR Vectorization ####
rm yesSIMDdata.txt
foreach numThreads (1 2 4 6 8 16)
	foreach NUMS (1000 10000 100000 1000000 2000000 15000000 30000000 60000000)
		icpc -o prog source.cpp -lm -DNUMT=$numThreads -DNUMS=$NUMS -openmp -align -qopt-report=3 -qopt-report-phase=vec
		./prog >>! yesSIMDdata.txt
	end
end
mv source.optrpt yesSIMD.optrpt
