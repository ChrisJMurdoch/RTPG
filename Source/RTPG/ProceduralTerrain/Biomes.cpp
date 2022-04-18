
#include "Biomes.h"

#include "../PerlinNoise/PerlinNoise.h"
#include"../Math/Math.h"

float Biomes::height(float x, float y, int octaves)
{
	// Interpolate different biomes
	float precip = precipitation(x, y);
	float heat = precipitation(x + 1000, y + 1000);
	return Math::lerp(
		Math::lerp( // Cold biomes
			temperate(x, y, 12) / 2,
			tundra(x, y, 12),
			precip
		),
		Math::lerp( // Hot biomes
			desert(x, y, 12) / 5,
			marsh(x, y, 12),
			precip
		),
		heat
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
	float const BLEND_SHARPNESS = 0.96f, // -1 : 1
				BLEND_FREQ = 0.2;
	float precipitation = Math::fBm(x*BLEND_FREQ, y*BLEND_FREQ, PerlinNoise::standard, 2, 1);
	precipitation = Math::kSigmoid(precipitation, BLEND_SHARPNESS);
	precipitation = Math::usgn(precipitation);
	return precipitation;
}

float Biomes::temperate(float x, float y, int octaves)
{
	// Return basic, fractal-layered Perlin Noise
	return Math::fBm(x, y, PerlinNoise::standard, octaves, 1);
}

float Biomes::tundra(float x, float y, int octaves)
{
	// Blend together Perlin Noise with ridged noise
	return Math::fBm(x, y, PerlinNoise::ridge,    4,       1) / 2 +
		   Math::fBm(x, y, PerlinNoise::standard, octaves, 1) / 2 - 0.2f;
}

float Biomes::marsh(float x, float y, int octaves)
{
	// Flattened bubble noise for marsh
	float marsh = Math::fBm(x*3, y*3, PerlinNoise::bubble, 1, 1);
	marsh = Math::kSigmoid(marsh +0.9f, 0.9f);

	// Diverged noise for mesas
	float mesa = Math::fBm(x * 3, y * 3, PerlinNoise::standard, 1, 1);
	mesa = Math::kSigmoid(mesa-0.2f, 0.95f);

	return ((marsh/60.0f) + (mesa/20.0f)) / 2.0f + 0.1f;
}

float horizontalSineRidge(float y)
{
	// Return sine-wave ridges
	return 2 - abs(sin(y*3.14))*2;
}

float Biomes::desert(float x, float y, int octaves)
{
	// Configurable parameters
	float const FREQ = 1.0f,
				WARP = 0.8f,
				WFREQ = 0.2,
				AMP = 0.225;

	// Get values for domain warp
	float dwX = Math::fBm(x*WFREQ*10+250, 3*y*WFREQ+250, PerlinNoise::standard, 2, 1);
	float dwY = Math::fBm(x*WFREQ*10+500, 3*y*WFREQ+500, PerlinNoise::standard, 2, 1);

	// Get two, separately domain-warped sine wave ridges
	float frontOne = horizontalSineRidge(y*FREQ + dwX*WARP) * AMP;
	float frontTwo = horizontalSineRidge((y+0.25)*FREQ + dwY*WARP) * AMP;

	// Create third layer of noise for interpolating between sine ridges
	float noise = Math::fBm(x*FREQ*1, y*FREQ*1, PerlinNoise::standard, 2, 1);

	// Push noise distribution to upper and lower ranges
	noise = Math::kSigmoid(noise, 0.75f);

	// Return noise based on noise interpolation 
	return Math::lerp(frontOne, frontTwo, Math::usgn(noise)) - 0.0f;
}
