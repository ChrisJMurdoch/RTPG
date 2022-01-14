
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

// Adapted from https://dhemery.github.io/DHE-Modules/technical/sigmoid/
float Math::kSigmoid(float x, float k)
{
    return ( x + k*x ) /
           ( -k + 2*k*abs(x) + 1 );
}

float Math::fBm(float x, float y, float(&sampler)(float, float), int octaves)
{
	static float const LACUNARITY=2, PERSISTANCE=0.41f;

	float value=0, max=0;
	float frequency=1, amplitude=1;
	for (int i=0; i<octaves; i++)
	{
		value += sampler(x*frequency, y*frequency) * amplitude;
		max += amplitude;

		frequency *= LACUNARITY;
		amplitude *= PERSISTANCE;
	}

	return value / max;
}

float Math::sgn(float t)
{
	return t*2 - 1;
}

float Math::usgn(float t)
{
	return t/2 + 0.5;
}
