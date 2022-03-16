
#pragma once

#include "CoreMinimal.h"

class RTPG_API Biomes
{
public:
	static float height(float x, float y, int octaves);
	static FLinearColor colour(float x, float y);

private:
	static float precipitation(float x, float y);
	static float temperate(float x, float y, int octaves);
	static float tundra(float x, float y, int octaves);
	static float marsh(float x, float y, int octaves);
	static float desert(float x, float y, int octaves);
};
