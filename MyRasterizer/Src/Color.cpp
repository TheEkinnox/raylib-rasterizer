#include "Color.h"

#include "Arithmetic.h"
#include "Interpolation.h"
#include "Arithmetic.h"

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
			static_cast<uint8_t>(LibMath::min(static_cast<short>(m_r) + p_other.m_r, 255)),
			static_cast<uint8_t>(LibMath::min(static_cast<short>(m_g) + p_other.m_g, 255)),
			static_cast<uint8_t>(LibMath::min(static_cast<short>(m_b) + p_other.m_b, 255)),
			static_cast<uint8_t>(LibMath::min(static_cast<short>(m_a) + p_other.m_a, 255))
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
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_r) * p_vec3.m_x, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_g) * p_vec3.m_y, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_b) * p_vec3.m_z, 0, 255)),
			m_a
		};
	}

	Color Color::operator*(const float p_scalar) const
	{
		return {
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_r) * p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_g) * p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_b) * p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_a) * p_scalar, 0, 255))
		};
	}

	Color Color::operator/(const float p_scalar) const
	{
		return {
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_r) / p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_g) / p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_b) / p_scalar, 0, 255)),
			static_cast<uint8_t>(LibMath::clamp(static_cast<float>(m_a) / p_scalar, 0, 255))
		};
	}

	Color& Color::operator+=(const Color& p_other)
	{
		return *this = *this + p_other;
	}

	Color Color::lerp(const Color& a, const Color& b, const float t)
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

	Color Color::bLerp(const Color* p_colors[4], LibMath::Vector2 p_point,
		const LibMath::Vector2& p_min, const LibMath::Vector2& p_max)
	{
		/*
		* https://en.wikipedia.org/wiki/Bilinear_interpolation
		*/
		Color c1, c2, c3;

		p_point = LibMath::Vector2(LibMath::clamp(p_point.m_x, p_min.m_x, p_max.m_x),	//clamp the point's x value
			LibMath::clamp(p_point.m_y, p_min.m_y, p_max.m_y));	//clamp the point's y value

		const LibMath::Vector2 delta = p_max - p_min;

		float t;
		if (delta.m_x != 0.f)
			t = (p_point.m_x - p_min.m_x) / delta.m_x;
		else
			t = 0;

		c1 = lerp(*p_colors[0], *p_colors[1], t);		// Q11 + t*Q21
		c2 = lerp(*p_colors[2], *p_colors[3], t);		// Q12 + t*Q22

		if (delta.m_y != 0.f)
			t = (p_point.m_y - p_min.m_y) / delta.m_y;
		else
			t = 0;

		c3 = lerp(c1, c2, t);						// R12 + t*R22

		return c3;
	}

	Color Color::bLerp(const Color* p_colors[4], LibMath::Vector2 p_point)
	{
		return bLerp(p_colors, p_point, LibMath::Vector2(0), LibMath::Vector2(1));
	}

}
