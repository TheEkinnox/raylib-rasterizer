#pragma once
#include <cstdint>
#include "Vector/Vector3.h"

namespace My
{
	struct Color
	{
		Color() = default;
		Color(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a) :
				m_r(p_r), m_g(p_g), m_b(p_b), m_a(p_a) {}

		Color(LibMath::Vector3 p_rgb, uint8_t p_a) :
			m_r(static_cast<uint8_t>(p_rgb.m_x)), 
			m_g(static_cast<uint8_t>(p_rgb.m_y)), 
			m_b(static_cast<uint8_t>(p_rgb.m_z)), 
			m_a(p_a) {}

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

		static Color	lerp(const Color& a, const Color& b, float t);
	};
}

