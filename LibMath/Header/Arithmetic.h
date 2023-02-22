#ifndef __LIBMATH__ARITHMETIC_H__
#define __LIBMATH__ARITHMETIC_H__
#include <limits>

namespace LibMath
{
	float	ceil(float);				// Return lowest integer value higher than or equal to the parameter
	float	clamp(float, float, float);	// Return parameter limited by the given range
	float	floor(float value);				// Return highest integer value lower than or equal to the parameter

	/**
	 * \brief Approximates the square root of a given value up to the given precision.
	 * \param value The value to compute the square root of
	 * \param precision The target precision of the computed square root
	 * \param maxSteps The maximum number of babylonian steps to perform
	 * \return An approimation of the square root of the given value
	 */
	float	squareRoot(float value, float precision = std::numeric_limits<float>::epsilon(), size_t maxSteps = 16);
	float	wrap(float, float, float);	// Return parameter as value inside the given range
	float	min(float, float);			// Return smallest parameter
	float	max(float, float);			// Return largest parameter
	float	abs(float);					// Return absolute value of the parameter

	bool	floatEquals(float a, float b);
}

#endif // !__LIBMATH__ARITHMETIC_H__
