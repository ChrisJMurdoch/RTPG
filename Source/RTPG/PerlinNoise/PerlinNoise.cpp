
#include "PerlinNoise.h"

#include "../Math/Math.h"

#include <cmath>

float PerlinNoise::standard(float x, float y, int seed)
{
	// Grid coordinates
	int16_t X=std::floor(x), Y=std::floor(y);

	// Grid-relative sample coordinates
	FVector2D sample(x-X, y-Y);

	// Get relative vectors
	FVector2D rBL = sample - FVector2D(0, 0);
	FVector2D rBR = sample - FVector2D(1, 0);
	FVector2D rTL = sample - FVector2D(0, 1);
	FVector2D rTR = sample - FVector2D(1, 1);

	// Generate pseudorandom vectors
	FVector2D pBL = randomVector(X,   Y,   seed);
	FVector2D pBR = randomVector(X+1, Y,   seed);
	FVector2D pTL = randomVector(X,   Y+1, seed);
	FVector2D pTR = randomVector(X+1, Y+1, seed);

	// Calculate dot products
	float dBL = FVector2D::DotProduct(pBL, rBL);
	float dBR = FVector2D::DotProduct(pBR, rBR);
	float dTL = FVector2D::DotProduct(pTL, rTL);
	float dTR = FVector2D::DotProduct(pTR, rTR);

	// Interpolate using smootherstep
	float xLerp=Math::smootherstep(sample.X), yLerp=Math::smootherstep(sample.Y);
	float dB = Math::lerp(dBL, dBR, xLerp);
	float dT = Math::lerp(dTL, dTR, xLerp);
	float d  = Math::lerp(dB,  dT,  yLerp);

	return d;
}

float PerlinNoise::bubble(float x, float y, int seed)
{
	return abs( standard(x, y, seed) )*2 - 1;
}

float PerlinNoise::ridge(float x, float y, int seed)
{
	return -bubble(x, y, seed);
}

FVector2D PerlinNoise::randomVector(int16_t X, int16_t Y, int32_t seed)
{
	// Change to unsigned integers
	uint16_t x=(1<<15)+X, y=(1<<15)+Y;

	// Calculate hash of combined coordinates
	int32_t hashedSeed = Math::intHash(seed);
	uint32_t combined = (static_cast<uint32_t>(x)<<16) | (y);
	int32_t h1=Math::intHash(combined^hashedSeed), h2=Math::intHash(combined^hashedSeed^0x11111111);

	// Generate unit vector
	FVector2D vector(h1, h2);
	vector.Normalize();
	return vector;
}
