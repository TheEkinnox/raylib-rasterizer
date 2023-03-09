#include <iostream>
#include <stdexcept>
#include <string>
#include "raylib.h"
#include "stb_image.h"
#include "Texture.h"
#include "Color.h"
#include "Vertex.h"
#include "Arithmetic.h"
#include "Vector/Vector2.h"

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

My::Color My::Texture::getPixelColorBlerp(	float p_x, float p_y,
											LibMath::Vector2 p_deltaTriangleBounds) const
{
	if (p_deltaTriangleBounds.m_x == 0 || p_deltaTriangleBounds.m_y == 0) // TODO : texture blerp delta (0,0) deal with them with lerp 
		return m_pixels[static_cast<size_t>(LibMath::round(p_y) * m_width + LibMath::round(p_x))];

	LibMath::Vector2 deltaRatio = LibMath::Vector2(	static_cast<float>(m_height), 
													static_cast<float>(m_width)) / p_deltaTriangleBounds;

	LibMath::Vector2 min(LibMath::round(p_x - deltaRatio.m_x / 2),	LibMath::round(p_y - deltaRatio.m_y / 2));
	LibMath::Vector2 max(LibMath::round(p_x + deltaRatio.m_x / 2),	LibMath::round(p_y + deltaRatio.m_y / 2));

	min.m_x = min.m_x < 0 ? 0 : min.m_x;
	min.m_y = min.m_y < 0 ? 0 : min.m_x;
	max.m_x = max.m_x >= m_width  ? static_cast<float>(m_width) - 1.0f : max.m_x;
	max.m_y = max.m_y >= m_height ? static_cast<float>(m_height) - 1.0f : max.m_y;

	const Color* colorsAround[4]
	{
		&m_pixels[static_cast<size_t>(min.m_y * m_width + min.m_x)],	//top left
		&m_pixels[static_cast<size_t>(min.m_y * m_width + max.m_x)],	//top right
		&m_pixels[static_cast<size_t>(max.m_y * m_width + min.m_x)],	//bot left
		&m_pixels[static_cast<size_t>(max.m_y * m_width + max.m_x)]		//bot right
	};

	return Color::bLerp(colorsAround, LibMath::Vector2(p_x, p_y), min, max);
}



