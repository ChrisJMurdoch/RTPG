
#pragma once

#include "CoreMinimal.h"

class RTPG_API Math
{
public:
	static uint32_t intHash(uint32_t t);

	static uint32_t intCombine(uint16_t t1, uint16_t t2);

	static float lerp(float a, float b, float t);

	static float smootherstep(float t);

	static float kSigmoid(float x, float k);

	static float fBm(float x, float y, float(&sampler)(float, float, int), int octaves, int seed);

	static float sgn(float t);

	static float usgn(float t);
}; 
