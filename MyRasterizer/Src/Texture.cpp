#include <iostream>
#include <stdexcept>
#include <string>
#include "raylib.h"
#include "stb_image.h"
#include "Texture.h"
#include "Color.h"
#include "Vertex.h"

My::Texture::Texture(const uint32_t p_width, const uint32_t p_height)
{
	this->m_width = p_width;
	this->m_height = p_height;

	// Cast width to size_t before multiplying to avoid overflow
	const size_t numPixels = static_cast<size_t>(m_width) * m_height;

	m_pixels = numPixels == 0 ? nullptr : new Color[numPixels];

	for (size_t i = 0; i < numPixels; i++)
		m_pixels[i] = Color::black;
}

My::Texture::Texture(const char* p_imagePath)
{
	const Image image = LoadImage(p_imagePath);

	// TODO: Throw image load failed exception
	if (image.data == nullptr)
	{
		std::cout << "Image failed to load";
		throw;
	}

	m_width = static_cast<uint32_t>(image.width);
	m_height = static_cast<uint32_t>(image.height);

	::Color* pixels = LoadImageColors(image);

	const size_t textureSize = static_cast<size_t>(m_width) * m_height;

	m_pixels = new Color[textureSize];

	for (size_t i = 0; i < textureSize; i++)
		m_pixels[i] = { pixels[i].r, pixels[i].g, pixels[i].b, pixels[i].a };
}

void My::Texture::setPixelColor(const uint32_t p_x, const uint32_t p_y, const Color& p_c)
{
	if (p_x < 0 || p_x >= m_width || p_y < 0 || p_y >= m_height)
		throw std::out_of_range("Pixel at coordinates " + std::to_string(p_x) +
			", " + std::to_string(p_y) + " is not in the texture");

	const size_t index = p_x + p_y * this->m_width;

	m_pixels[index] = p_c;
}


uint32_t My::Texture::getWidth() const
{
	return m_width;
}

uint32_t My::Texture::getHeight() const
{
	return m_height; 
}

My::Color My::Texture::getPixelColor(const uint32_t p_x, const uint32_t p_y) const
{
	return m_pixels[p_y * m_width + p_x];
}


