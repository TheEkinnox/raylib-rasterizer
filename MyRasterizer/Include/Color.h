#pragma once
#include <cstdint>
#include "Vector/Vector3.h"

namespace My
{
	struct Color
	{
		Color() = default;
		/// <summary>
		/// Creates a RGBA color with values clamped [0, 255]
		/// </summary>
		/// <param name="p_r">Red value</param>
		/// <param name="p_g">Green value</param>
		/// <param name="p_b">Blue value</param>
		/// <param name="p_a">Alpha/Transparency value</param>
		Color(const uint8_t p_r, const uint8_t p_g, const uint8_t p_b, const uint8_t p_a) :
			m_r(p_r), m_g(p_g), m_b(p_b), m_a(p_a) {}

		/// <summary>
		/// Creates a RGBA color with RGB vector and Alpha value with values clamped [0, 255]
		/// </summary>
		/// <param name="p_rgb">RGB Vector3</param>
		/// <param name="p_a">Alpha/Transparency value</param>
		Color(const LibMath::Vector3 p_rgb, const uint8_t p_a) :
			m_r(static_cast<uint8_t>(p_rgb.m_x)),
			m_g(static_cast<uint8_t>(p_rgb.m_y)),
			m_b(static_cast<uint8_t>(p_rgb.m_z)),
			m_a(p_a) {}

		/// <summary>
		/// Multiplies the red, green and blue components by the given amount
		/// </summary>
		/// <param name="f">The value by which each element should be multiplied</param>
		Color rgbMultiply(float f) const;

		/// <summary>
		/// Get the Red, Green and Blue values in a Vector3
		/// </summary>
		LibMath::Vector3 rgb()const;

		/// <summary>
		/// Adds each element of two colors
		/// </summary>
		/// <param name="p_other">The color to add to the current one</param>
		Color operator+(const Color& p_other) const;

		/// <summary>
		/// Multiplies each element of two colors
		/// </summary>
		/// <param name="p_other">The color to multiply the current one by</param>
		Color operator*(const Color& p_other) const;

		/// <summary>
		/// Multiplies each element of the color by the corresponding component
		/// of a given vector (ie: r * x, g * y, b * z)
		/// </summary>
		/// <param name="p_vec3">The vector by which each element should be multiplied</param>
		Color operator*(const LibMath::Vector3& p_vec3) const;

		/// <summary>
		/// Multiplies each element by the given amount
		/// </summary>
		/// <param name="p_scalar">The value by which each element should be multiplied</param>
		Color operator*(float p_scalar) const;

		/// <summary>
		/// Divides each element by the given amount
		/// </summary>
		/// <param name="p_scalar">The value by which each element should be divided</param>
		Color operator/(float p_scalar) const;

		uint8_t m_r;
		uint8_t m_g;
		uint8_t m_b;
		uint8_t m_a;

		static const Color black;
		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;

		/// <summary>
		/// Interpolates from Color a to Color b by t amount
		/// </summary>
		/// <param name="a">Color</param>
		/// <param name="b">Color</param>
		/// <param name="t"> :  t=0 => Color = a; t=1 => Color = b; </param>
		/// <returns></returns>
		static Color	lerp(const Color& a, const Color& b, float t);

		Color& operator*=(const Color& color);
	};
}

