#include "Color.h"
#include "Interpolation.h"

namespace My
{
	const Color Color::black	= { 0, 0, 0, 255 };
	const Color Color::white	= { 255, 255, 255, 255 };
	const Color Color::red		= { 255, 0, 0, 255 };
	const Color Color::green	= { 0, 255, 0, 255 };
	const Color Color::blue		= { 0, 0, 255, 255 };

	Color Color::rgbMultiply(const float f) const
	{
		return Color(static_cast<uint8_t>(m_r * f),
			static_cast<uint8_t>(m_g * f),
			static_cast<uint8_t>(m_b * f),
			m_a);
	}

	LibMath::Vector3 Color::rgb() const
	{
		return LibMath::Vector3(m_r, m_g, m_b);
	}

	Color Color::operator+(const Color& p_other) const
	{
		return {
			static_cast<uint8_t>(static_cast<short>(m_r) + p_other.m_r),
			static_cast<uint8_t>(static_cast<short>(m_g) + p_other.m_g),
			static_cast<uint8_t>(static_cast<short>(m_b) + p_other.m_b),
			static_cast<uint8_t>(static_cast<short>(m_a) + p_other.m_a)
		};
	}

	Color Color::operator*(const Color& p_other) const
	{
		return {
			static_cast<uint8_t>(static_cast<short>(m_r) * p_other.m_r / 255),
			static_cast<uint8_t>(static_cast<short>(m_g) * p_other.m_g / 255),
			static_cast<uint8_t>(static_cast<short>(m_b) * p_other.m_b / 255),
			static_cast<uint8_t>(static_cast<short>(m_a) * p_other.m_a / 255)
		};
	}

	Color Color::operator*(const LibMath::Vector3& p_vec3) const
	{
		return {
			static_cast<uint8_t>(static_cast<float>(m_r) * p_vec3.m_x),
			static_cast<uint8_t>(static_cast<float>(m_g) * p_vec3.m_y),
			static_cast<uint8_t>(static_cast<float>(m_b) * p_vec3.m_z),
			m_a
		};
	}

	Color Color::operator*(const float p_scalar) const
	{
		return {
			static_cast<uint8_t>(static_cast<float>(m_r) * p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_g) * p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_b) * p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_a) * p_scalar)
		};
	}

	Color Color::operator/(const float p_scalar) const
	{
		return {
			static_cast<uint8_t>(static_cast<float>(m_r) / p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_g) / p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_b) / p_scalar),
			static_cast<uint8_t>(static_cast<float>(m_a) / p_scalar)
		};
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

	Color& Color::operator*=(const Color& color)
	{
		m_r = (m_r * color.m_r) / 255;
		m_g = (m_g * color.m_g) / 255;
		m_b = (m_b * color.m_b) / 255;
		m_a = (m_a * color.m_a) / 255;

		return *this;
	}
}
