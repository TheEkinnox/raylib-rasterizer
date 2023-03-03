#include "Color.h"
#include "Interpolation.h"

namespace My
{
	const Color Color::black	= { 0, 0, 0, 255 };
	const Color Color::white	= { 255, 255, 255, 255 };
	const Color Color::red		= { 255, 0, 0, 255 };
	const Color Color::green	= { 0, 255, 0, 255 };
	const Color Color::blue		= { 0, 0, 255, 255 };

	Color Color::rgbMultiply(float f) const
	{
		return Color(	static_cast<uint8_t>(m_r * f), 
						static_cast<uint8_t>(m_g * f), 
						static_cast<uint8_t>(m_b * f), 
						m_a);
	}

	LibMath::Vector3 Color::rgb() const
	{
		return LibMath::Vector3(m_r, m_g, m_b);
	}

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
}
