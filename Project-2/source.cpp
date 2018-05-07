#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

// constants:
const double G = 6.67300e-11;   // m^3 / ( kg s^2 )
const double EARTH_MASS = 5.9742e24;     // kg
const double EARTH_DIAMETER = 12756000.32;   // meters
const double TIMESTEP = 1.0; // secs

#define NUMBODIES 100
#define NUMSTEPS 200


struct body{
	float mass;
	float x, y, z;            // position
	float vx, vy, vz;         // velocity
	float fx, fy, fz;         // forces
	float xnew, ynew, znew;
	float vxnew, vynew, vznew;
};

typedef struct body Body;

Body Bodies[NUMBODIES];

// function prototypes:
float GetDistanceSquared( Body *, Body * );
float GetUnitVector( Body *, Body *, float *, float *, float * );
float Ranf( float, float );
int Ranf( int, int );

 

 
float GetDistanceSquared(Body *bi, Body *bj){
	float dx = bi->x - bj->x;
	float dy = bi->y - bj->y;
	float dz = bi->z - bj->z;

	return dx*dx + dy*dy + dz*dz;
}

float GetUnitVector(Body *from, Body *to, float *ux, float *uy, float *uz){
	float dx = to->x - from->x;
	float dy = to->y - from->y;
	float dz = to->z - from->z;
	float d = sqrt( dx*dx + dy*dy + dz*dz );

	if( d > 0. ){
		dx /= d;
		dy /= d;
		dz /= d;
	}

	*ux = dx;
	*uy = dy;
	*uz = dz;

	return d;
}

 

 

float Ranf(float low, float high){
	float r = (float) rand();         // 0 - RAND_MAX

	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

 

int Ranf(int ilow, int ihigh){
	float low = (float)ilow;
	float high = (float)ihigh + 0.9999f;

	return (int)(  Ranf(low,high) );
}



/*
	* Coarse + Static Computation
*/
double megaBodiesCSCompute(){
	double startTime = omp_get_wtime( );

	for(int t = 0; t < NUMSTEPS; t++){
		#pragma omp parallel for schedule(static), private(t)
		for(int i = 0; i < NUMBODIES; i++){
			float fx = 0.;
			float fy = 0.;
			float fz = 0.;
			Body *bi = &Bodies[i];

			for(int j = 0; j < NUMBODIES; j++){
				if(j == i)
					continue;

				Body *bj = &Bodies[j];
				float rsqd = GetDistanceSquared(bi, bj);

				if(rsqd > 0.0){
					float f = G * bi->mass * bj->mass / rsqd;
					float ux, uy, uz;
					GetUnitVector( bi, bj,   &ux, &uy, &uz );
					fx += f * ux;
					fy += f * uy;
					fz += f * uz;
				}
			}


			float ax = fx / Bodies[i].mass;
			float ay = fy / Bodies[i].mass;
			float az = fz / Bodies[i].mass;

			Bodies[i].xnew = Bodies[i].x + Bodies[i].vx*TIMESTEP + 0.5*ax*TIMESTEP*TIMESTEP;
			Bodies[i].ynew = Bodies[i].y + Bodies[i].vy*TIMESTEP + 0.5*ay*TIMESTEP*TIMESTEP;
			Bodies[i].znew = Bodies[i].z + Bodies[i].vz*TIMESTEP + 0.5*az*TIMESTEP*TIMESTEP;


			Bodies[i].vxnew = Bodies[i].vx + ax*TIMESTEP;
			Bodies[i].vynew = Bodies[i].vy + ay*TIMESTEP;
			Bodies[i].vznew = Bodies[i].vz + az*TIMESTEP;
		}

 

		// setup the state for the next animation step:
		for(int i = 0; i < NUMBODIES; i++){
			Bodies[i].x = Bodies[i].xnew;
			Bodies[i].y = Bodies[i].ynew;
			Bodies[i].z = Bodies[i].znew;
			Bodies[i].vx = Bodies[i].vxnew;
			Bodies[i].vy = Bodies[i].vynew;
			Bodies[i].vz = Bodies[i].vznew;
		}

	} 

	double endTime = omp_get_wtime();
	double megaBodies = ((float)(NUMBODIES*NUMBODIES*NUMSTEPS)/(endTime-startTime)/1000000.0);

	return megaBodies;
}

/*
	* Coarse + Dynamic Computation
*/
double megaBodiesCDCompute(){
	double startTime = omp_get_wtime( );

	for(int t = 0; t < NUMSTEPS; t++){
		#pragma omp parallel for schedule(dynamic), private(t)
		for(int i = 0; i < NUMBODIES; i++){
			float fx = 0.;
			float fy = 0.;
			float fz = 0.;
			Body *bi = &Bodies[i];

			for(int j = 0; j < NUMBODIES; j++){
				if(j == i)
					continue;

				Body *bj = &Bodies[j];
				float rsqd = GetDistanceSquared(bi, bj);

				if(rsqd > 0.0){
					float f = G * bi->mass * bj->mass / rsqd;
					float ux, uy, uz;
					GetUnitVector( bi, bj,   &ux, &uy, &uz );
					fx += f * ux;
					fy += f * uy;
					fz += f * uz;
				}
			}


			float ax = fx / Bodies[i].mass;
			float ay = fy / Bodies[i].mass;
			float az = fz / Bodies[i].mass;

			Bodies[i].xnew = Bodies[i].x + Bodies[i].vx*TIMESTEP + 0.5*ax*TIMESTEP*TIMESTEP;
			Bodies[i].ynew = Bodies[i].y + Bodies[i].vy*TIMESTEP + 0.5*ay*TIMESTEP*TIMESTEP;
			Bodies[i].znew = Bodies[i].z + Bodies[i].vz*TIMESTEP + 0.5*az*TIMESTEP*TIMESTEP;


			Bodies[i].vxnew = Bodies[i].vx + ax*TIMESTEP;
			Bodies[i].vynew = Bodies[i].vy + ay*TIMESTEP;
			Bodies[i].vznew = Bodies[i].vz + az*TIMESTEP;
		}

 

		// setup the state for the next animation step:
		for(int i = 0; i < NUMBODIES; i++){
			Bodies[i].x = Bodies[i].xnew;
			Bodies[i].y = Bodies[i].ynew;
			Bodies[i].z = Bodies[i].znew;
			Bodies[i].vx = Bodies[i].vxnew;
			Bodies[i].vy = Bodies[i].vynew;
			Bodies[i].vz = Bodies[i].vznew;
		}

	} 

	double endTime = omp_get_wtime();
	double megaBodies = ((float)(NUMBODIES*NUMBODIES*NUMSTEPS)/(endTime-startTime)/1000000.0);

	return megaBodies;
}

/*
	* Fine + Static Computation
*/
double megaBodiesFSCompute(){
	double startTime = omp_get_wtime( );

	for(int t = 0; t < NUMSTEPS; t++){
		for(int i = 0; i < NUMBODIES; i++){
			float fx = 0.;
			float fy = 0.;
			float fz = 0.;
			Body *bi = &Bodies[i];

			#pragma omp parallel for schedule(static), private(t,i), reduction(+:fx),reduction(+:fy),reduction(+:fz)
			for(int j = 0; j < NUMBODIES; j++){
				if(j == i)
					continue;

				Body *bj = &Bodies[j];
				float rsqd = GetDistanceSquared(bi, bj);

				if(rsqd > 0.0){
					float f = G * bi->mass * bj->mass / rsqd;
					float ux, uy, uz;
					GetUnitVector( bi, bj,   &ux, &uy, &uz );
					fx += f * ux;
					fy += f * uy;
					fz += f * uz;
				}
			}


			float ax = fx / Bodies[i].mass;
			float ay = fy / Bodies[i].mass;
			float az = fz / Bodies[i].mass;

			Bodies[i].xnew = Bodies[i].x + Bodies[i].vx*TIMESTEP + 0.5*ax*TIMESTEP*TIMESTEP;
			Bodies[i].ynew = Bodies[i].y + Bodies[i].vy*TIMESTEP + 0.5*ay*TIMESTEP*TIMESTEP;
			Bodies[i].znew = Bodies[i].z + Bodies[i].vz*TIMESTEP + 0.5*az*TIMESTEP*TIMESTEP;


			Bodies[i].vxnew = Bodies[i].vx + ax*TIMESTEP;
			Bodies[i].vynew = Bodies[i].vy + ay*TIMESTEP;
			Bodies[i].vznew = Bodies[i].vz + az*TIMESTEP;
		}

 

		// setup the state for the next animation step:
		for(int i = 0; i < NUMBODIES; i++){
			Bodies[i].x = Bodies[i].xnew;
			Bodies[i].y = Bodies[i].ynew;
			Bodies[i].z = Bodies[i].znew;
			Bodies[i].vx = Bodies[i].vxnew;
			Bodies[i].vy = Bodies[i].vynew;
			Bodies[i].vz = Bodies[i].vznew;
		}

	}

	double endTime = omp_get_wtime();
	double megaBodies = ((float)(NUMBODIES*NUMBODIES*NUMSTEPS)/(endTime-startTime)/1000000.0);

	return megaBodies;
}

/*
	* Fine + Dynamic Computation
*/
double megaBodiesFDCompute(){
	double startTime = omp_get_wtime( );

	for(int t = 0; t < NUMSTEPS; t++){
		for(int i = 0; i < NUMBODIES; i++){
			float fx = 0.;
			float fy = 0.;
			float fz = 0.;
			Body *bi = &Bodies[i];

			#pragma omp parallel for schedule(dynamic), private(t,i), reduction(+:fx),reduction(+:fy),reduction(+:fz)			
			for(int j = 0; j < NUMBODIES; j++){
				if(j == i)
					continue;

				Body *bj = &Bodies[j];
				float rsqd = GetDistanceSquared(bi, bj);

				if(rsqd > 0.0){
					float f = G * bi->mass * bj->mass / rsqd;
					float ux, uy, uz;
					GetUnitVector( bi, bj,   &ux, &uy, &uz );
					fx += f * ux;
					fy += f * uy;
					fz += f * uz;
				}
			}


			float ax = fx / Bodies[i].mass;
			float ay = fy / Bodies[i].mass;
			float az = fz / Bodies[i].mass;

			Bodies[i].xnew = Bodies[i].x + Bodies[i].vx*TIMESTEP + 0.5*ax*TIMESTEP*TIMESTEP;
			Bodies[i].ynew = Bodies[i].y + Bodies[i].vy*TIMESTEP + 0.5*ay*TIMESTEP*TIMESTEP;
			Bodies[i].znew = Bodies[i].z + Bodies[i].vz*TIMESTEP + 0.5*az*TIMESTEP*TIMESTEP;


			Bodies[i].vxnew = Bodies[i].vx + ax*TIMESTEP;
			Bodies[i].vynew = Bodies[i].vy + ay*TIMESTEP;
			Bodies[i].vznew = Bodies[i].vz + az*TIMESTEP;
		}

 

		// setup the state for the next animation step:
		for(int i = 0; i < NUMBODIES; i++){
			Bodies[i].x = Bodies[i].xnew;
			Bodies[i].y = Bodies[i].ynew;
			Bodies[i].z = Bodies[i].znew;
			Bodies[i].vx = Bodies[i].vxnew;
			Bodies[i].vy = Bodies[i].vynew;
			Bodies[i].vz = Bodies[i].vznew;
		}

	} 

	double endTime = omp_get_wtime();
	double megaBodies = ((float)(NUMBODIES*NUMBODIES*NUMSTEPS)/(endTime-startTime)/1000000.0);

	return megaBodies;
}


int main(int argc, char *argv[ ]){
	#ifndef _OPENMP
		fprintf( stderr, "OpenMP is not available\n" );
		return 1;
	#endif

	omp_set_num_threads(NUMT);
	// int numProcessors = omp_get_num_procs();
	// fprintf(stderr, "Have %d processors.\n", numProcessors);

	for(int i = 0; i < NUMBODIES; i++){
		Bodies[i].mass = EARTH_MASS  * Ranf( 0.5f, 10.f );
		Bodies[i].x = EARTH_DIAMETER * Ranf( -100.f, 100.f );
		Bodies[i].y = EARTH_DIAMETER * Ranf( -100.f, 100.f );
		Bodies[i].z = EARTH_DIAMETER * Ranf( -100.f, 100.f );
		Bodies[i].vx = Ranf( -100.f, 100.f );;
		Bodies[i].vy = Ranf( -100.f, 100.f );;
		Bodies[i].vz = Ranf( -100.f, 100.f );;
	};

	double megaBodiesCS = megaBodiesCSCompute();
	double megaBodiesCD = megaBodiesCDCompute();
	double megaBodiesFS = megaBodiesFSCompute();
	double megaBodiesFD = megaBodiesFDCompute();


	// printf("Using %d threads\n", NUMT);
	// printf("Performance in mega Bodies Compared Per Second: %8.2lf \n", megaBodiesCompPerSec);
	// printf("Time in microseconds: %lf \n", (1e9) * (endTime - startTime));	

	/* Used to output to file to then be parsed and excel sheet generated */
	printf("%d,%8.2lf,%8.2lf,%8.2lf,%8.2lf \n", NUMT, megaBodiesCS, megaBodiesCD, megaBodiesFS, megaBodiesFD);

	return 0;
}