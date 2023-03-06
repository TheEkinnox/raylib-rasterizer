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
		/// <param name="p_a">Alpha/Transpanrecy value</param>
		Color(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a) :
				m_r(p_r), m_g(p_g), m_b(p_b), m_a(p_a) {}

		/// <summary>
		/// Creates a RGBA color with RGB vector and Alpha value with values clamped [0, 255]
		/// </summary>
		/// <param name="p_rgb">RGB Vector3</param>
		/// <param name="p_a">Alpha/Transpanrecy value</param>
		Color(LibMath::Vector3 p_rgb, uint8_t p_a) :
			m_r(static_cast<uint8_t>(p_rgb.m_x)), 
			m_g(static_cast<uint8_t>(p_rgb.m_y)), 
			m_b(static_cast<uint8_t>(p_rgb.m_z)), 
			m_a(p_a) {}

		/// <summary>
		/// Get the Red, Green and Blue values in a Vector3
		/// </summary>
		Color rgbMultiply(float) const; 
		LibMath::Vector3 rgb()const;


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
	};
}

