
#include "Math.h"

#include <cmath>

int combineInts(int t1, int t2)
{
	return t1*12345 + t2;
}

int Math::intHash(int t)
{
	// Modified from: https://stackoverflow.com/a/12996028
	t = ((t >> 16) ^ t) * 0x45d9f3b;
	t = ((t >> 16) ^ t) * 0x45d9f3b;
	t = (t >> 16) ^ t;
	static int const resolution = 1000;
	return (t % (resolution+1)) - (resolution/2);
}

int Math::intHash(int t1, int t2)
{
	return intHash( combineInts(t1, t2) );
}

int Math::intHash(int t1, int t2, int t3)
{
	return intHash( combineInts( t1, combineInts(t2, t3) ) );
}

float Math::lerp(float a, float b, float t)
{
	return a + (b-a)*t;
}

float Math::smootherstep(float t)
{
	return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3);
}
