/*
 * MathUtil
 * - nuetzliche mathematische Funktionen
 * - zufaellige Zahlenberechnung
 */
#ifndef MathUtil_h
#define MathUitl_h
constexpr double PI = 3.14159265359;

class MathUtil
{
public:

	// float i~[fromMin,fromMax] -> i~[toMin, toMax]
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

	static float clamp(float x, float min, float max)
	{
		if (x > max) x = max;
		if (x < min) x = min;
		return x;
	}

	static double clamp(double x, double min, double max)
	{
		if (x > max) x = max;
		if (x < min) x = min;
		return x;
	}
};

#endif