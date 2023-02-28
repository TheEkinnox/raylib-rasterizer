#pragma once
#include <cstdint>

namespace My
{
	struct Color
	{
		Color() = default;
		Color(uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a) :
				m_r(p_r), m_g(p_g), m_b(p_b), m_a(p_a) {}

		uint8_t m_r;
		uint8_t m_g;
		uint8_t m_b;
		uint8_t m_a;

		static const Color black;
		static const Color white;
		static const Color red;
		static const Color green;
		static const Color blue;
	};
}

