
#pragma once

#include "CoreMinimal.h"

class RTPG_API PerlinNoise
{
public:
	static float sample(float x, float y);
	static float fBm(float x, float y, int octaves);

public:
	static FVector2D randomVector(int16_t X, int16_t Y);
};
