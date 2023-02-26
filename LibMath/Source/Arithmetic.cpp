#include "Arithmetic.h"

#include <limits>

namespace LibMath
{
	float ceil(const float num)
	{
		const auto intPart = static_cast<float>(static_cast<int>(num));

		if (intPart < num)
			return intPart + 1;

		return intPart;
	}

	float clamp(const float val, const float a, const float b)
	{
		const float min = a < b ? a : b;
		const float max = a > b ? a : b;

		if (val < min)
			return min;

		if (val > max)
			return max;

		return val;
	}

	float floor(const float value)
	{
		const float intPart = static_cast<float>(static_cast<int>(value));

		if (intPart > value)
			return intPart - 1;

		return intPart;
	}

	float squareRoot(const float value, float precision, const size_t maxSteps)
	{
		if (value < 0)
			return NAN;

		if (value == 0.f)
			return 0.f;

		if (precision == 0.f)
			precision = std::numeric_limits<float>::epsilon();
		else
			precision = abs(precision);

		float sqrt = value >= 4.f ? value / 2.f :
			value < .5f ? 1.f / (1 - value) :
			value < 1.f ? 1.f / value : value;

		// Keep repeating until the approximation is close enough to the real value
		for (size_t i = 0; (maxSteps == 0 || i < maxSteps) && abs(sqrt * sqrt - value) > precision; i++)
			sqrt = .5f * (sqrt + value / sqrt); // Babylonian step - cf. https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Heron's_method

		return sqrt;
	}

	float wrap(const float val, const float min, const float max)
	{
		return val - (max - min) * floor((val - min) / (max - min));
	}

	float min(const float x, const float y)
	{
		return x < y ? x : y;
	}

	float max(const float x, const float y)
	{
		return x > y ? x : y;
	}

	float abs(const float value)
	{
		return value < 0 ? -value : value;
	}

	// adapted from https://stackoverflow.com/a/15012792
	bool floatEquals(const float a, const float b)
	{
		const float maxXYOne = max(max(1.0f, abs(a)),	abs(b));

		return abs(a - b) <= std::numeric_limits<float>::epsilon() * maxXYOne;
	}
}