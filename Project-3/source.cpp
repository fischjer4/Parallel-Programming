#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>


struct s{
	float value;
	// Uncomment for Fix #1	
	int pad[NUMPAD];
} Array[4];

int main(int argc, char *argv[ ]){
	#ifndef _OPENMP
		fprintf( stderr, "OpenMP is not available\n" );
		return 1;
	#endif

	omp_set_num_threads(NUMT);
	int someBigNumber = 1000000000;

	double startTime = omp_get_wtime();
	#pragma omp parallel for
	for(int i = 0; i < 4; i++){
		// Uncomment for Fix #2
		// float tmp = Array[i].value;
		for(int j = 0; j < someBigNumber; j++ ){
			// Uncomment for Fix #1	
			Array[i].value = Array[i].value + 2;

			// Uncomment for Fix #2
			// tmp = tmp + 2.;
		}
		// Uncomment for Fix #2
		// Array[i].value = tmp;
	}

	double endTime = omp_get_wtime();
	// Print out for Fix #1: Padding Solution
	printf("%d,%d,%.0f\n", NUMT, NUMPAD, (1e6) * (endTime - startTime));	

	// Used for Fix #2: The private variable solution
	// printf("%d,%.0f\n", NUMT, (1e6) * (endTime - startTime));	

	return 0;
}