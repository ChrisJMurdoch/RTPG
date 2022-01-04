
#pragma once

#include "CoreMinimal.h"

class RTPG_API Math
{
public:
	static int intHash(int t);
	static int intHash(int t1, int t2);
	static int intHash(int t1, int t2, int t3);

	static float lerp(float a, float b, float t);

	static float smootherstep(float t);
};
