#include <iostream>
#include <stdexcept>
#include <string>
#include "raylib.h"
#include "Texture.h"

#include "Arithmetic.h"
#include "Color.h"

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

My::Texture::Texture(const Texture& p_other)
{
	m_width = p_other.m_width;
	m_height = p_other.m_height;

	const size_t textureSize = static_cast<size_t>(m_width) * m_height;

	m_pixels = textureSize == 0 ? nullptr : new Color[textureSize];

	for (size_t i = 0; i < textureSize; i++)
		m_pixels[i] = p_other.m_pixels[i];
}

My::Texture::Texture(Texture&& p_other) noexcept
{
	m_pixels = p_other.m_pixels;
	m_width = p_other.m_width;
	m_height = p_other.m_height;

	p_other.m_pixels = nullptr;
	p_other.m_width = p_other.m_height = 0;
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

	m_pixels = textureSize == 0 ? nullptr : new Color[textureSize];

	for (size_t i = 0; i < textureSize; i++)
		m_pixels[i] = { pixels[i].r, pixels[i].g, pixels[i].b, pixels[i].a };

	UnloadImageColors(pixels);
	UnloadImage(image);
}

My::Texture::~Texture()
{
	delete[] m_pixels;
}

My::Texture& My::Texture::operator=(const Texture& p_other)
{
	if (this == &p_other)
		return *this;

	delete[] m_pixels;

	m_width = p_other.m_width;
	m_height = p_other.m_height;

	const size_t textureSize = static_cast<size_t>(m_width) * m_height;

	m_pixels = textureSize == 0 ? nullptr : new Color[textureSize];

	for (size_t i = 0; i < textureSize; i++)
		m_pixels[i] = p_other.m_pixels[i];

	return *this;
}

My::Texture& My::Texture::operator=(Texture&& p_other) noexcept
{
	if (this == &p_other)
		return *this;

	delete[] m_pixels;

	m_pixels = p_other.m_pixels;
	m_width = p_other.m_width;
	m_height = p_other.m_height;

	p_other.m_pixels = nullptr;
	p_other.m_width = p_other.m_height = 0;

	return *this;
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

My::Color My::Texture::getPixelColorBlerp(const float p_x, const float p_y,
                                          const LibMath::Vector2 p_deltaRatio) const
{
	if (LibMath::floatEquals(p_deltaRatio.m_x, 0)
		|| LibMath::floatEquals(p_deltaRatio.m_y, 0)) // TODO : texture blerp delta (0,0) deal with them with lerp
		return m_pixels[static_cast<size_t>(LibMath::round(p_y) * static_cast<float>(m_width) + LibMath::round(p_x))];

	const LibMath::Vector2 deltaRatio = LibMath::Vector2(	static_cast<float>(m_width),
	                                                        static_cast<float>(m_height)) / p_deltaRatio;

	LibMath::Vector2 min(LibMath::round(p_x - deltaRatio.m_x / 2),	LibMath::round(p_y - deltaRatio.m_y / 2));
	LibMath::Vector2 max(LibMath::round(p_x + deltaRatio.m_x / 2),	LibMath::round(p_y + deltaRatio.m_y / 2));

	const float floatWidth = static_cast<float>(m_width);
	const float floatHeight = static_cast<float>(m_height);

	min.m_x = min.m_x < 0 ? 0 : min.m_x;
	min.m_y = min.m_y < 0 ? 0 : min.m_y;
	max.m_x = max.m_x >= floatWidth ? floatWidth - 1.0f : max.m_x;
	max.m_y = max.m_y >= floatHeight ? floatHeight - 1.0f : max.m_y;

	const Color* colorsAround[4]
	{
		&m_pixels[static_cast<size_t>(min.m_y * floatWidth + min.m_x)],	//top left
		&m_pixels[static_cast<size_t>(min.m_y * floatWidth + max.m_x)],	//top right
		&m_pixels[static_cast<size_t>(max.m_y * floatWidth + min.m_x)],	//bot left
		&m_pixels[static_cast<size_t>(max.m_y * floatWidth + max.m_x)]	//bot right
	};

	return Color::bLerp(colorsAround, LibMath::Vector2(p_x, p_y), min, max);
}

void My::Texture::setPixelColor(const uint32_t p_x, const uint32_t p_y, const Color& p_c)
{
	if (p_x < 0 || p_x >= m_width || p_y < 0 || p_y >= m_height)
		throw std::out_of_range("Pixel at coordinates " + std::to_string(p_x) +
			", " + std::to_string(p_y) + " is not in the texture");

	const size_t index = p_x + p_y * this->m_width;

	m_pixels[index] = p_c;
}
