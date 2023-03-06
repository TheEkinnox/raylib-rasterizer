#pragma once
#include <cstdint>

namespace My
{
	struct Color;

	class Texture
	{
	public:
		Texture() = delete;
		Texture(uint32_t p_width, uint32_t p_height);
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture(const char* image_path);
		~Texture() = default;

		Texture&	operator=(const Texture&) = default;
		Texture&	operator=(Texture&&) = default;

		uint32_t	getWidth() const;
		uint32_t	getHeight() const;
		Color		getPixelColor(uint32_t p_x, uint32_t p_y) const;
		void		setPixelColor(uint32_t p_x, uint32_t p_y, const Color& p_c);
	private:
		uint32_t	m_width;
		uint32_t	m_height;
		Color*		m_pixels;
		
	};

}

