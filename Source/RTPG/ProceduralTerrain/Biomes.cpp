
#include "Biomes.h"

#include "../PerlinNoise/PerlinNoise.h"
#include"../Math/Math.h"

float Biomes::height(float x, float y, int octaves)
{
	// Interpolate different biomes
	return Math::lerp(
		desert(x, y, octaves) / 5,
		temperate(x, y, octaves),
		precipitation(x, y)
	);
}

FLinearColor Biomes::colour(float x, float y)
{
	// Interpolate different biomes
	float p = precipitation(x, y);
	float h = precipitation(x+1000, y+1000);
	return FLinearColor(h, 0.0f, p, p); // Pack Heat in Red channel, Precipitation in Blue channel
}

float Biomes::precipitation(float x, float y)
{
	// Get biome interpolants
	float const BLEND_SHARPNESS = 0.9f, // -1 : 1
				BLEND_FREQ = 0.333;
	float precipitation = Math::fBm(x*BLEND_FREQ, y*BLEND_FREQ, PerlinNoise::standard, 2, 1);
	precipitation = Math::kSigmoid(precipitation, BLEND_SHARPNESS);
	precipitation = Math::usgn(precipitation);
	return precipitation;
}

float Biomes::temperate(float x, float y, int octaves)
{
	return Math::fBm(x, y, PerlinNoise::standard, 10, 1);
}

float horizontalSineRidge(float y)
{
	return 2 - abs(sin(y*3.14))*2;
}

float Biomes::desert(float x, float y, int octaves)
{
	float const FREQ = 1.0f,
				WARP = 0.8f,
				WFREQ = 0.2,
				AMP = 0.225;

	float dwX = Math::fBm(x*WFREQ*10+250, 3*y*WFREQ+250, PerlinNoise::standard, 2, 1);
	float dwY = Math::fBm(x*WFREQ*10+500, 3*y*WFREQ+500, PerlinNoise::standard, 2, 1);

	float frontOne = horizontalSineRidge(y*FREQ + dwX*WARP) * AMP;
	float frontTwo = horizontalSineRidge((y+0.25)*FREQ + dwY*WARP) * AMP;

	float noise = Math::fBm(x*FREQ*1, y*FREQ*1, PerlinNoise::standard, 2, 1);
	noise = Math::kSigmoid(noise, 0.75f);

	return Math::lerp(frontOne, frontTwo, Math::usgn(noise)) - 3.0f;
}
