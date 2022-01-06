
#pragma once

#include "CoreMinimal.h"

class RTPG_API Math
{
public:
	static int intHash(int t);

	static int intCombine(int t1, int t2);

	static float lerp(float a, float b, float t);

	static float smootherstep(float t);
};
