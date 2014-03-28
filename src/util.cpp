#include "util.h"
#include <cstdlib>
#include <ctime>

double RandRange(double min, double max) 
{
	srand((unsigned)time(0));

	double r = (double)rand();
	r /= (double)RAND_MAX;

	double range = max - min;
	r *= range;
	r += min;

	return r;
}
