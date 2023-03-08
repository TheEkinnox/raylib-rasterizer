#include "Color.h"
#include "Interpolation.h"
#include "Arithmetic.h"
#include "Vector/Vector2.h"

namespace My
{
	const Color Color::black	= { 0, 0, 0, UINT8_MAX };
	const Color Color::white	= { UINT8_MAX, UINT8_MAX, UINT8_MAX, UINT8_MAX };
	const Color Color::red		= { UINT8_MAX, 0, 0, UINT8_MAX };
	const Color Color::green	= { 0, UINT8_MAX, 0, UINT8_MAX };
	const Color Color::blue		= { 0, 0, UINT8_MAX, UINT8_MAX };

	Color& Color::aditionClamp(const LibMath::Vector3& p_rgb)
	{
		this->m_r = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_r) + p_rgb.m_x,
														0.0f, UINT8_MAX));
		this->m_g = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_g) + p_rgb.m_y,
														0.0f, UINT8_MAX));		
		this->m_b = static_cast<uint8_t>(LibMath::clamp(static_cast<float>(this->m_b) + p_rgb.m_z,
														0.0f, UINT8_MAX));

		return *this;
	}

	Color& Color::blend(const Color& p_dest)
	{
		float srcAlphaPercent = static_cast<float>(this->m_a) / UINT8_MAX;
		float destAlphaPercent = static_cast<float>(p_dest.m_a) / UINT8_MAX * (1 - srcAlphaPercent);

		this->m_r = static_cast<uint8_t>(	static_cast<float>(this->m_r) * srcAlphaPercent + 
											static_cast<float>(p_dest.m_r) * destAlphaPercent);
		this->m_g = static_cast<uint8_t>(	static_cast<float>(this->m_g) * srcAlphaPercent + 
											static_cast<float>(p_dest.m_g) * destAlphaPercent);
		this->m_b = static_cast<uint8_t>(	static_cast<float>(this->m_b) * srcAlphaPercent + 
											static_cast<float>(p_dest.m_b) * destAlphaPercent);
		this->m_a = UINT8_MAX;

		return *this;
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
			static_cast<uint8_t>(static_cast<short>(m_r) * p_other.m_r / UINT8_MAX),
			static_cast<uint8_t>(static_cast<short>(m_g) * p_other.m_g / UINT8_MAX),
			static_cast<uint8_t>(static_cast<short>(m_b) * p_other.m_b / UINT8_MAX),
			static_cast<uint8_t>(static_cast<short>(m_a) * p_other.m_a / UINT8_MAX)
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
	Color Color::bLerp(	const Color p_colors[4], LibMath::Vector2 p_point, 
						const LibMath::Vector2& p_min, const LibMath::Vector2& p_max)
	{
		/*
		* https://en.wikipedia.org/wiki/Bilinear_interpolation
		*/
		Color c1, c2, c3;

		p_point = LibMath::Vector2(	LibMath::clamp(p_point.m_x, p_min.m_x, p_max.m_x),	//clamp the point's x value
									LibMath::clamp(p_point.m_y, p_min.m_y, p_max.m_y));	//clamp the point's y value

		LibMath::Vector2 delta = p_max - p_min;

		if (delta.m_x == 0 || delta.m_y == 0)
			throw "lmoa"; // TODO : Exception Class for Color blerp()

		float t = (p_point.m_x - p_min.m_x) / delta.m_x;
		c1 = lerp(p_colors[0], p_colors[1], t);		// Q11 + t*Q21
		c2 = lerp(p_colors[2], p_colors[3], t);		// Q12 + t*Q22

		t = (p_point.m_y - p_min.m_y) / delta.m_y;			
		c3 = lerp(c1, c2, t);						// R12 + t*R22

		return c3;
	}
	Color Color::bLerp(const Color p_colors[4], LibMath::Vector2 p_point) 
	{
		return Color::bLerp(p_colors, p_point, LibMath::Vector2(0), LibMath::Vector2(1));
	}
}
