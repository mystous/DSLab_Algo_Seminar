#include "TimeMeasurement.h"
#include <stdio.h>
#include <time.h>

clock_t start_time, end_time;

void start_time_measurement()
{
	start_time = clock();
}

void end_time_measurement()
{
	end_time = clock();
	printf("Time measurement: %f\n", ((double)(end_time - start_time)) / CLOCKS_PER_SEC);
}