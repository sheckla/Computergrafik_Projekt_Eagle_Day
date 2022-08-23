/*
 * MathUtil
 * - nuetzliche mathematische Funktionen
 * - zufaellige Zahlenberechnung
 *
 */

#ifndef MathUtil_h
#define MathUitl_h

class MathUtil
{
public:
	static float remapBounds(float i, float fromMin, float fromMax, float toMin, float toMax)
	{
		return (i - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
	}

	// [0, 1]
	static float randPercentage(float min = 0.0f, float max = 1.0f)
	{

		float random = ((float)rand() / (float)RAND_MAX);
		return remapBounds(random, 0, 1, min, max);
	}

	static bool randBool()
	{
		if (randPercentage() > 0.5) return true;
		return false;
	}

};

#endif