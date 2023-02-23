#ifndef __LIBMATH__MATRIX__MATRIX4_H__
#define __LIBMATH__MATRIX__MATRIX4_H__

#include "MatrixInternal.h"

namespace LibMath
{
	class Matrix4x2 : public Matrix
	{
	public:
					Matrix4x2(length_t, length_t) = delete;
					Matrix4x2();
		explicit	Matrix4x2(float scalar);
	};

	class Matrix4x3 : public Matrix
	{
	public:
					Matrix4x3(length_t, length_t) = delete;
					Matrix4x3();
		explicit	Matrix4x3(float scalar);
	};

	class Matrix4x4 : public Matrix
	{
	public:
					Matrix4x4(length_t, length_t) = delete;
					Matrix4x4();
		explicit	Matrix4x4(float scalar);
	};

	typedef Matrix4x4 Matrix4;
}

#ifdef __LIBMATH__VECTOR__VECTOR4_H__
#include "Matrix4Vector4Operation.h"
#endif // __LIBMATH__MATRIX_H__

#endif // !__LIBMATH__MATRIX__MATRIX4_H__
