#include <time.h>

fm::Time getCurrentTime()
{
	timespec timet;
	clock_gettime(CLOCK_MONOTONIC,&timet);
	return fm::milliseconds((double)timet.tv_sec*1000.0+(double)timet.tv_nsec/1000000.0);
}