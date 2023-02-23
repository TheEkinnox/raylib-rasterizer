#ifndef __LIBMATH__MATRIX__MATRIX3_H__
#define __LIBMATH__MATRIX__MATRIX3_H__

#include "MatrixInternal.h"

namespace LibMath
{
	class Matrix3x2 : public Matrix
	{
	public:
					Matrix3x2(length_t, length_t) = delete;
					Matrix3x2();
		explicit	Matrix3x2(float scalar);
	};

	class Matrix3x3 : public Matrix
	{
	public:
					Matrix3x3(length_t, length_t) = delete;
					Matrix3x3();
		explicit	Matrix3x3(float scalar);
	};

	class Matrix3x4 : public Matrix
	{
	public:
					Matrix3x4(length_t, length_t) = delete;
					Matrix3x4();
		explicit	Matrix3x4(float scalar);
	};

	typedef Matrix3x3 Matrix3;
}

#endif // !__LIBMATH__MATRIX__MATRIX3_H__
