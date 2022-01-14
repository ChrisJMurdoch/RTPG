
#pragma once

#include "CoreMinimal.h"

class RTPG_API Biomes
{
public:
	static float sample(float x, float y, int octaves);

private:
	static float temperate(float x, float y, int octaves);
	static float desert(float x, float y, int octaves);
};
