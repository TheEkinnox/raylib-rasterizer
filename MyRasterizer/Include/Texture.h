#pragma once

namespace My
{
	/*
	Texture: repr�sente une image et m_a donc les membres suivants priv�s
	unsigned int m_width
	unsigned int m_height
	Color* m_pixels

	La Texture ne doit pas avoir de constructeur par d�faut(= delete)
	Elle doit avoir un constructeur qui prend en param�tre la largeur et la hauteur, et remplit l�image de noir
	
	Elle doit avoir une fonction SetPixelColor(unsigned int x, unsigned int y, const Color& c)
	*/

	class Color;

	class Texture
	{
	public:
		Texture(unsigned int p_x, unsigned int p_y);
		~Texture() = default;

		void SetPixelColor(unsigned int p_x, unsigned int p_y, const Color& p_c);

	private:
		unsigned int m_width;
		unsigned int m_height;
		Color* m_pixels;

	};

}

