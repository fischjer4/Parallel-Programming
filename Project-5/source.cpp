#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>


/* Init global arrays */
float A[NUMS], C[NUMS];


int main(int argc, char *argv[ ]){
	#ifndef _OPENMP
		fprintf( stderr, "OpenMP is not available\n" );
		return 1;
	#endif

  srand (time(NULL));
  for (int i = 0; i < NUMS; i++){
    A[i] = rand() % 100000 + 1;
    C[i] = rand() % 100000 + 1;
  }

	omp_set_num_threads(NUMT);
	double startTime = omp_get_wtime();

	#pragma omp parallel for
	for(int i = 0; i < NUMS; i++){
    C[i] = sqrt(A[i]);
	}

	double endTime = omp_get_wtime();
	printf("%d,%d,%.0f\n", NUMT, NUMS, (1e6) * (endTime - startTime));

	return 0;
}
