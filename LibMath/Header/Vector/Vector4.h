#ifndef __LIBMATH__VECTOR__VECTOR4_H__
#define __LIBMATH__VECTOR__VECTOR4_H__

namespace LibMath
{
	class Radian;

	class Vector4
	{
	public:
						Vector4() = default;
		explicit		Vector4(float);									// set all the component to the same value
						Vector4(float, float, float, float);			// set all component individually
						Vector4(Vector4 const& other) = default;
						Vector4(Vector4&& other) = default;
						~Vector4() = default;

		static Vector4	zero();											// return a vector with all its component set to 0
		static Vector4	one();											// return a vector with all its component set to 1
		static Vector4	up();											// return a unit vector pointing upward
		static Vector4	down();											// return a unit vector pointing downward
		static Vector4	left();											// return a unit vector pointing left
		static Vector4	right();										// return a unit vector pointing right
		static Vector4	front();										// return a unit vector pointing forward
		static Vector4	back();											// return a unit vector pointing backward

		Vector4&		operator=(Vector4 const& other) = default;
		Vector4&		operator=(Vector4&& other) = default;

		float&			operator[](int);								// return this vector component value
		float			operator[](int) const;							// return this vector component value

		Vector4&		operator+=(Vector4 const& other);						// addition component wise
		Vector4&		operator-=(Vector4 const& other);						// subtraction component wise
		Vector4&		operator*=(Vector4 const& other);						// multiplication component wise
		Vector4&		operator/=(Vector4 const& other);						// division component wise

		Vector4&		operator+=(float const&);						// add a value to all components
		Vector4&		operator-=(float const&);						// subtract a value from all components
		Vector4&		operator*=(float const&);						// multiply all components by a value
		Vector4&		operator/=(float const&);						// divide all components by a value

		Radian			angleFrom(Vector4 const& other) const;				// return smallest angle between 2 vectors
		Radian			signedAngleFrom(Vector4 const& other,
						Vector4 const& axis) const;						// return signed angle between 2 vectors around 

		Vector4			cross(Vector4 const&) const;					// return a copy of the cross product result

		float			distanceFrom(Vector4 const&) const;				// return distance between 2 points
		float			distanceSquaredFrom(Vector4 const&) const;		// return square value of the distance between 2 points
		float			distance2DFrom(Vector4 const&) const;			// return the distance between 2 points on the X-Y axis only
		float			distance2DSquaredFrom(Vector4 const&) const;	// return the square value of the distance between 2 points points on the X-Y axis only

		float			dot(Vector4 const&) const;						// return dot product result

		bool			isLongerThan(Vector4 const&) const;				// return true if this vector magnitude is greater than the other
		bool			isShorterThan(Vector4 const&) const;			// return true if this vector magnitude is less than the other

		bool			isUnitVector() const;							// return true if this vector magnitude is 1

		float			magnitude() const;								// return vector magnitude
		float			magnitudeSquared() const;						// return square value of the vector magnitude

		void			normalize();									// scale this vector to have a magnitude of 1
		Vector4			normalized() const;								// returns this vector scaled to have a magnitude of 1

		void			projectOnto(Vector4 const&);					// project this vector onto an other

		void			reflectOnto(Vector4 const&);					// reflect this vector by an other

		void			rotate(Radian, Radian, Radian);					// rotate this vector using euler angle apply in the z, x, y order
		void			rotate(Radian, Vector4 const&);					// rotate this vector around an arbitrary axis
		//void			rotate(Quaternion const&); todo quaternion		// rotate this vector using a quaternion rotor

		void			scale(Vector4 const&);							// scale this vector by a given factor

		std::string		string() const;									// return a string representation of this vector
		std::string		stringLong() const;								// return a verbose string representation of this vector

		void			translate(Vector4 const&);						// offset this vector by a given distance

		float m_x = 0;
		float m_y = 0;
		float m_z = 0;
		float m_w = 0;
	};
}

#ifdef __LIBMATH__MATRIX__MATRIX4_H__
#include "Matrix4Vector4Operation.h"
#endif // __LIBMATH__MATRIX__MATRIX4_H__

#endif // !__LIBMATH__VECTOR__VECTOR4_H__
