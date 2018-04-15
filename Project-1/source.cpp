#include <omp.h>
#include <stdio.h>
#include <math.h>


#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.

#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.

#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.

#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.

#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.

#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.

#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.

#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.


// ix,iy = 0 .. NUMNODES-1
float height(int ix, int iy){
	float x = (float)ix / (float)(NUMNODES-1);
	float y = (float)iy / (float)(NUMNODES-1);

	// the basis functions:
	float bu0 = (1.0-x) * (1.0-x) * (1.0-x);
	float bu1 = 3.0 * x * (1.0-x) * (1.0-x);
	float bu2 = 3.0 * x * x * (1.0-x);
	float bu3 = x * x * x;

	float bv0 = (1.0-y) * (1.0-y) * (1.0-y);
	float bv1 = 3.0 * y * (1.0-y) * (1.0-y);
	float bv2 = 3.0 * y * y * (1.0-y);
	float bv3 = y * y * y;

	// finally, we get to compute something:
	float top =       bu0 * ( bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03 )
					+ bu1 * ( bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13 )
					+ bu2 * ( bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23 )
					+ bu3 * ( bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33 );

	float bot =       bu0 * ( bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03 )
					+ bu1 * ( bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13 )
					+ bu2 * ( bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23 )
					+ bu3 * ( bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33 );

	/*
	  if the bottom surface sticks out above the top surface
	  then that contribution to the overall volume is negatiye
	*/ 
	return top - bot;	
}

int main(int argc, char *argv[]){
	#ifndef _OPENMP
		fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
		return 1;
	#endif

	// the area of a single full-sized tile:
	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
	float halfTileArea = (fullTileArea / 2);
	float quarterTileArea = (fullTileArea / 4);

	/*
	  sum up the weighted heights into the variable "volume"
	  using an OpenMP for loop and a reduction 
	*/
	omp_set_num_threads(NUMT);

	double volume = 0;
	
	// Assume a full tile
	double curArea = fullTileArea;
	double startTime = omp_get_wtime();
	int ix = 0;
	int iy = 0;
	#pragma omp parallel for private(ix,iy,curArea), reduction(+:volume)
	for(int i = 0; i < NUMNODES*NUMNODES; i++){
		ix = i % NUMNODES;
		iy = i / NUMNODES;
		
		// Assume a full tile	
		curArea = fullTileArea;

		// If on an edge
		if( (iy == 0 || iy == (NUMNODES - 1) ) || (ix == 0 || ix == (NUMNODES - 1) ) ){
			// If on a corner
			if( (ix == 0 || ix == NUMNODES - 1) && (iy == 0 || iy == NUMNODES - 1) ){
				curArea = quarterTileArea;
			}
			else{
				curArea = halfTileArea;				
			}
		}

		// Sum the newly obtained dv 
		volume += curArea * height(ix, iy);
	}
	double endTime = omp_get_wtime();
	double megaHeights = (double)NUMNODES*NUMNODES/(endTime-startTime)/1000000.0;

	// printf("Using %d threads\n", NUMT);
	// printf("Using %d nodes\n", NUMNODES);
	// printf("Performance in megaHeights: %8.2lf \n", megaHeights);
	// printf("Time in microseconds: %lf \n", (1e9) * (endTime - startTime));	
	// printf("Volume: %lf \n\n", volume);

	/* Used to output to file to then be parsed and excel sheet generated */
	printf("%d,%d,%8.2lf \n", NUMT,NUMNODES,megaHeights);

	return 0;
}