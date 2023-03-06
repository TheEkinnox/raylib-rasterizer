#include "Color.h"
#include "Interpolation.h"
#include "Arithmetic.h"

namespace My
{
	const Color Color::black	= { 0, 0, 0, 255 };
	const Color Color::white	= { 255, 255, 255, 255 };
	const Color Color::red		= { 255, 0, 0, 255 };
	const Color Color::green	= { 0, 255, 0, 255 };
	const Color Color::blue		= { 0, 0, 255, 255 };

	Color& Color::rgbMultiply(float f)
	{
		this->m_r = static_cast<uint8_t>(static_cast<float>(m_r * f));
		this->m_g = static_cast<uint8_t>(static_cast<float>(m_g * f));
		this->m_b = static_cast<uint8_t>(static_cast<float>(m_b * f));

		return *this;
	}

	Color& Color::rgbAdition(uint8_t i)
	{
		this->m_r += i;
		this->m_g += i;
		this->m_b += i;

		return *this;
	}

	Color& Color::rgbAditionClamp(const Color& other)
	{
		this->m_r = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_r) + static_cast<float>(other.m_r), 
														0.0f, 255.0f));
		this->m_g = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_g) + static_cast<float>(other.m_g),
														0.0f, 255.0f));		
		this->m_b = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_b) + static_cast<float>(other.m_b),
														0.0f, 255.0f));

		return *this;
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
