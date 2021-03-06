
#include "PerlinNoise.h"

#include "../Math/Math.h"

#include <cmath>

float PerlinNoise::standard(float x, float y, int seed)
{
	// Grid cell coordinates
	int16_t X=std::floor(x), Y=std::floor(y);

	// Sample coordinates within cell
	FVector2D sample(x-X, y-Y);

	// Get sample vectors relative to each corner of the grid cell
	FVector2D rBL = sample - FVector2D(0, 0);
	FVector2D rBR = sample - FVector2D(1, 0);
	FVector2D rTL = sample - FVector2D(0, 1);
	FVector2D rTR = sample - FVector2D(1, 1);

	// Generate pseudorandom vectors for each corner of the cell
	FVector2D pBL = randomVector(X,   Y,   seed);
	FVector2D pBR = randomVector(X+1, Y,   seed);
	FVector2D pTL = randomVector(X,   Y+1, seed);
	FVector2D pTR = randomVector(X+1, Y+1, seed);

	// Calculate dot products for each relative and pseudorandom vector
	float dBL = FVector2D::DotProduct(pBL, rBL);
	float dBR = FVector2D::DotProduct(pBR, rBR);
	float dTL = FVector2D::DotProduct(pTL, rTL);
	float dTR = FVector2D::DotProduct(pTR, rTR);

	// Interpolate between corner dot products using smootherstep
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

// Create a random 2D unit vector for X and Y coordinate

	FVector2D PerlinNoise::randomVector(int16_t X, int16_t Y, int32_t seed)
	{
		// Shift signed ints into positive range
		uint16_t x=(1<<15)+X, y=(1<<15)+Y;

		// Calculate hash of combined coordinates
		int32_t hashedSeed = Math::intHash(seed);
		uint32_t combinedCoordinates = (static_cast<uint32_t>(x)<<16) | (y);
		int32_t hash1=Math::intHash(combinedCoordinates^hashedSeed),
				hash2=Math::intHash(combinedCoordinates^hashedSeed^0x11111111);

		// Generate unit vector
		FVector2D vector(hash1, hash2);
		vector.Normalize();
		return vector;
	}
