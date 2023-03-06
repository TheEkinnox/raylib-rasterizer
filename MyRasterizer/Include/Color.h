#pragma once
#include <cstdint>

namespace My
{
	struct Color
	{
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

		Color& operator*=(const Color& color);
	};
}

