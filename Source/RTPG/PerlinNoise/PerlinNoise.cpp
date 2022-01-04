
#include "PerlinNoise.h"

#include "../Math/Math.h"

#include <cmath>

float PerlinNoise::sample(float x, float y, int seed)
{
	// Grid coordinates
	int X=std::floor(x), Y=std::floor(y);

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

float PerlinNoise::fBm(float x, float y, int octaves, int seed)
{
	static float const LACUNARITY=2, PERSISTANCE=0.41f;

	float value=0, max=0;
	float frequency=1, amplitude=1;
	for (int i=0; i<octaves; i++)
	{
		value += sample(x*frequency, y*frequency, seed) * amplitude;
		max += amplitude;

		frequency *= LACUNARITY;
		amplitude *= PERSISTANCE;
	}

	return value / max;
}

FVector2D PerlinNoise::randomVector(int X, int Y, int seed)
{
	FVector2D vector( Math::intHash(X, Y, seed), Math::intHash(X, Y, seed+1) );
	vector.Normalize();
	return vector;
}
