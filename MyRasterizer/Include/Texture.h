#pragma once
#include <cstdint>
#include "Vector/Vector2.h"

namespace My
{
	struct Color;

	class Texture
	{
	public:
					Texture() = delete;
					Texture(uint32_t p_width, uint32_t p_height);
					Texture(const Texture& p_other);
					Texture(Texture&& p_other) noexcept;
		explicit	Texture(const char* p_imagePath);
					~Texture();

		Texture&	operator=(const Texture&);
		Texture&	operator=(Texture&&) noexcept;

		uint32_t	getWidth() const;
		uint32_t	getHeight() const;
		Color		getPixelColor(uint32_t p_x, uint32_t p_y) const;
		Color		getPixelColorBlerp(	float p_x, float p_y, 
										LibMath::Vector2 p_deltaRatio) const;
		void		setPixelColor(uint32_t p_x, uint32_t p_y, const Color& p_c);
	private:
		uint32_t	m_width;
		uint32_t	m_height;
		Color*		m_pixels;
		
	};

}

