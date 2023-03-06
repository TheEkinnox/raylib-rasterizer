#include "Color.h"
#include "Interpolation.h"

namespace My
{
	const Color Color::black	= { 0, 0, 0, 255 };
	const Color Color::white	= { 255, 255, 255, 255 };
	const Color Color::red		= { 255, 0, 0, 255 };
	const Color Color::green	= { 0, 255, 0, 255 };
	const Color Color::blue		= { 0, 0, 255, 255 };

	Color Color::lerp(const Color& a, const Color& b, float t)
	{
		return
		{
			static_cast<uint8_t>(LibMath::lerp(a.m_r, b.m_r, t)),
			static_cast<uint8_t>(LibMath::lerp(a.m_g, b.m_g, t)),
			static_cast<uint8_t>(LibMath::lerp(a.m_b, b.m_b, t)),
			static_cast<uint8_t>(LibMath::lerp(a.m_a, b.m_a, t))
		};
	}

	Color& Color::operator*=(const Color& color)
	{
		m_r = (m_r * color.m_r) / 255;
		m_g = (m_g * color.m_g) / 255;
		m_b = (m_b * color.m_b) / 255;
		m_a = (m_a * color.m_a) / 255;

		return *this;
	}
}
