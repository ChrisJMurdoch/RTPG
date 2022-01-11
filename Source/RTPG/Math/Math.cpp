
#include "Math.h"

#include <cmath>

// half_avalanche hashing function from https://burtleburtle.net/bob/hash/integer.html
uint32_t Math::intHash(uint32_t a)
{
    a = (a+0x479ab41d) + (a<<8);
    a = (a^0xe4aa10ce) ^ (a>>5);
    a = (a+0x9942f0a6) - (a<<14);
    a = (a^0x5aedd67d) ^ (a>>3);
    a = (a+0x17bea992) + (a<<7);
    return a;
}

uint32_t Math::intCombine(uint16_t t1, uint16_t t2)
{
	return static_cast<uint32_t>(t1)<<16 | (t2);
}

float Math::lerp(float a, float b, float t)
{
	return a + (b-a)*t;
}

float Math::smootherstep(float t)
{
	return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3);
}
