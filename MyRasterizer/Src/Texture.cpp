#include "Texture.h"
#include "Color.h"

My::Texture::Texture(unsigned int p_x, unsigned int p_y)
{
	this->m_width = p_x;
	this->m_height = p_y;

	size_t numPixels = m_width * m_height;
	for (size_t i = 0; i < numPixels; i++)
		this->m_pixels[i] = Color();
}

void My::Texture::SetPixelColor(unsigned int p_x, unsigned int p_y, const Color& p_c)
{
	size_t index = p_x + p_y * this->m_width;

	if (index >= m_width * m_height)
		throw "lmoa"; // TODO: exeption Texture index out of range

	m_pixels[index] = p_c;
}
