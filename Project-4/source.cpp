#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int  NowYear;           // 2014 - 2019
int  NowMonth;          // 0 - 11

float NowPrecip;        // inches of rain per month
float NowTemp;          // temperature this month
float NowHeight;        // grain height in inches
int   NowNumDeer;        // current deer population
int LocustOutburst;
unsigned int seed = 0;

const float GRAIN_GROWS_PER_MONTH =             8.0; //inches
const float ONE_DEER_EATS_PER_MONTH =           0.5;

const float AVG_PRECIP_PER_MONTH =              6.0; //inches
const float AMP_PRECIP_PER_MONTH =              6.0;
const float RANDOM_PRECIP =                     2.0;

const float AVG_TEMP =                          50.0; //Farenheight
const float AMP_TEMP =                          20.0;
const float RANDOM_TEMP =                       10.0;

const float MIDTEMP =                           40.0;
const float MIDPRECIP =                         10.0;

const int CURYEAR 		=													2018;
/* this is when the program will stop */
const int STOPYEAR		=													CURYEAR + 5;




/*
	* Custom random float generator
*/
float Ranf(float low, float high, unsigned int *seed){
    float r = (float) rand_r(seed); // 0 - RAND_MAX
    return( low + r * ( high - low ) / (float)RAND_MAX );
}
/*
	* Returns the angle
*/
float getAngle(){
	return (30.0 * (float)NowMonth + 15.0) * ( M_PI / 180.0 );
}
/*
	* Figures out the new temperature level with
	  respect to the current month
*/
float getNewTemp(){
	float temp = AVG_TEMP - AMP_TEMP * cos( getAngle() );
	return temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP, &seed);
}
/*
	* returns the damage that is applied to the NowHeight
	  by the Locust outburst. Note that 0 is returned if
		there is not a locust outburst
*/
float getLocustDestruction(){
	return LocustOutburst ? Ranf(-NowHeight, -0.5, &seed) : 0.0 ;
}
/*
	* Figures out the new precipitation level with
	  respect to the current month
*/
float getNewPrecip(){
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( getAngle() );
	float newPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP, &seed);

	return newPrecip >= 0.0 ? newPrecip : 0.0;
}
/*
	* This function is incharge of handling the how much
	  the grain grows per month
	* Rules:
		-  If conditions are good, grain will grow by GRAIN_GROWS_PER_MONTH
		-  If conditions are not good, grain won't grow
*/
void Locust(){
	/* end the program when the year is STOPYEAR */
	while (NowYear <= STOPYEAR){

		int wasLocust = 0;
		/* If harvest season, possibility of locust */
		if (NowMonth >= 4 && NowMonth <= 8){
			/* if there is a +75% chance of locust, assume there was */
			if (Ranf(0.0, 1.0, &seed) > 0.75)
				wasLocust = 1;
		}

		/* Checking For Locust Outburst Barrier */
		#pragma omp barrier

		/* Done Computing Barrier */
		#pragma omp barrier

		LocustOutburst = wasLocust;

		/* Done Assigning Barrier */
		#pragma omp barrier

		/* Done Printing Barrier */
		#pragma omp barrier
	}
}
/*
	* This function is incharge of handling the number of graindeer
	  in the simulation
	* Rules:
		- The Carrying Capacity of the graindeer is the number of
		  inches of height of the grain
		- If the number of graindeer exceeds this value at the end
		  of a month, decrease the number of graindeer by one
		- If the number of graindeer is less than this value at the
		  end of a month, increase the number of graindeer by one
*/
void GrainDeer(){
	/* end the program when the year is STOPYEAR */
	while(NowYear <= STOPYEAR){
		/* Checking For Locust Outburst Barrier */
		#pragma omp barrier

		float carryingCapacity = NowHeight;
		int tmpNowNumDeer = NowNumDeer;
		if (tmpNowNumDeer > carryingCapacity){
			tmpNowNumDeer--;
		}
		else if(tmpNowNumDeer < carryingCapacity){
			tmpNowNumDeer++;
		}

		/* Done Computing Barrier */
		#pragma omp barrier

		NowNumDeer = tmpNowNumDeer >= 0 ? tmpNowNumDeer : 0;

		/* Done Assigning Barrier */
		#pragma omp barrier

		/* Done Printing Barrier */
		#pragma omp barrier

	}
}
/*
	* This function is incharge of handling the how much
	  the grain grows per month
	* Rules:
		-  If conditions are good, grain will grow by GRAIN_GROWS_PER_MONTH
		-  If conditions are not good, grain won't grow
*/
void GrainGrowth(){
	/* end the program when the year is STOPYEAR */
	while (NowYear <= STOPYEAR){
		/* Checking For Locust Outburst Barrier */
		#pragma omp barrier

		float tempDifference = NowTemp - MIDTEMP;
		float precipDifference = NowPrecip - MIDPRECIP;
		float temperatureFactor = exp( -1.0 * pow(tempDifference / 10.0 , 2));
		float precipitationFactor = exp( -1.0 * pow(precipDifference / 10.0 , 2));

		float tmpNowHeight = NowHeight;
		tmpNowHeight += temperatureFactor * precipitationFactor * GRAIN_GROWS_PER_MONTH;
		tmpNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
		/* If there is a locust outburst, the grain is effected */
		tmpNowHeight += getLocustDestruction();

		/* Done Computing Barrier */
		#pragma omp barrier

		NowHeight = tmpNowHeight > 0.0 ? tmpNowHeight : 0.0;

		/* Done Assigning Barrier */
		#pragma omp barrier

		/* Done Printing Barrier */
		#pragma omp barrier
	}
}
/*
	* This function is incharge of printing the current
	  state of the simulation, and calculating the new
		temperature and precipitation levels after a month
		has passed
	* Rules:
		-  If conditions are good, grain will grow by GRAIN_GROWS_PER_MONTH
		-  If conditions are not good, grain won't grow
*/
void Watcher(){
	while (NowYear <= STOPYEAR) {
		/* Checking For Locust Outburst Barrier */
		#pragma omp barrier

		int newMonth = NowMonth;
		int newYear = NowYear;
		float newTemp = getNewTemp();
		float newPrecip = getNewPrecip();

		newMonth++;
		if (newMonth > 11){
			newMonth = 0;
			newYear++;
		}

		/* Done Computing Barrier */
		#pragma omp barrier

		NowMonth = newMonth;
		NowYear = newYear;
		NowTemp = newTemp;
		NowPrecip = newPrecip;

		/* Done Assigning Barrier */
		#pragma omp barrier

		/* uncomment the below for generateExcelData.py use, and comment out printf below it */
		/* inches are converted to centimeters, and Farenheight to Celcius */
		printf("%d,%f,%f,%f,%d,%d\n", NowMonth, 2.54*NowPrecip, (5.0/9.0)*(NowTemp-32.0), 2.54*NowHeight, NowNumDeer, LocustOutburst);

		// printf("Month: %d,\t Year: %d,\t Precipitation: %f,\t Temperature: %f,\t Height: %f,\t NumDeer: %d,\t Locust: %s\n",
	 	// 		NowMonth, NowYear, NowPrecip, NowTemp, NowHeight, NowNumDeer, (LocustOutburst ? "True" : "False"));

		/* Done Printing Barrier */
		#pragma omp barrier
	}
}






int main(int argc, char *argv[ ]){
	#ifndef _OPENMP
		fprintf( stderr, "OpenMP is not available\n" );
		return 1;
	#endif

	/* Starting simulation values */
	NowHeight =  1.0;
	NowNumDeer =  1.0;
	NowMonth =    0;
	LocustOutburst = 0;
	NowYear  = CURYEAR;

	omp_set_num_threads(4);
	#pragma omp parallel sections
	{
			#pragma omp section
			{
					 Locust();
			}
			#pragma omp section
			{
	         GrainDeer();
	    }
	    #pragma omp section
	    {
	         GrainGrowth();
	    }
	    #pragma omp section
	    {
	         Watcher();
	    }
	    // implied barrier: all sections must complete before we get here
	}

	return 0;
}
