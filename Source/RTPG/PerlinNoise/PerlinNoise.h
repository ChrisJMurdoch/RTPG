
#pragma once

#include "CoreMinimal.h"

class RTPG_API PerlinNoise
{
public:
	static float standard(float x, float y, int seed);
	static float bubble(float x, float y, int seed);
	static float ridge(float x, float y, int seed);

public:
	static FVector2D randomVector(int16_t X, int16_t Y, int32_t seed);
};
