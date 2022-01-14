
#include "Biomes.h"

#include "../PerlinNoise/PerlinNoise.h"
#include"../Math/Math.h"

float Biomes::sample(float x, float y, int octaves)
{
	// Get biome interpolants
	float const BLEND_SHARPNESS = 0.9f, // -1 : 1
				BLEND_FREQ = 0.333;
	float precipitation = Math::fBm(x*BLEND_FREQ, y*BLEND_FREQ, PerlinNoise::standard, 2);
	precipitation = Math::kSigmoid(precipitation, BLEND_SHARPNESS);

	// Interpolate different biomes
	return Math::lerp(
		desert(x+5000, y+5000, octaves) / 5,
		temperate(x+10000, y+10000, octaves),
		0
	);
}

float Biomes::temperate(float x, float y, int octaves)
{
	return Math::fBm(x, y, PerlinNoise::standard, 2);
}

float horizontalSineRidge(float y)
{
	return 2 - abs(sin(y*3.14))*2;
}

float Biomes::desert(float x, float y, int octaves)
{
	float const FREQ = 1.5,
				WARP = 1,
				WFREQ = 0.2,
				AMP = 0.225;

	float dwX = Math::fBm(x*WFREQ*10+250, y*WFREQ+250, PerlinNoise::standard, 2);
	float dwY = Math::fBm(x*WFREQ*10+500, y*WFREQ+500, PerlinNoise::standard, 2);

	float frontOne = horizontalSineRidge(y*FREQ + dwX*WARP) * AMP;
	float frontTwo = horizontalSineRidge((y+0.25)*FREQ + dwY*WARP) * AMP;

	float noise = Math::fBm(x*FREQ*0.5, y*FREQ*0.5, PerlinNoise::standard, 2);
	noise = Math::kSigmoid(noise, 0.5f);

	return Math::lerp(frontOne, frontTwo, Math::usgn(noise));
}
