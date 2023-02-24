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

		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
	};
}

