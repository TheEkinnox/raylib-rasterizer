#pragma once

namespace My
{
	/*
	Color: contient 4 membres m_r, m_g, m_b, m_a pour chacun des canaux de couleur
	Chaque composante est représentée par un unsigned char
	0 pour aucune intensité, 255 pour l'intensité maximale. 

	Ex : (m_r, m_g, m_b, m_a) = (0, 0, 255, 255) représente la couleur bleue
	*/

	class Color
	{
	public:
		Color() = default;
		Color(unsigned char p_r, unsigned char p_g, unsigned char p_b, unsigned char p_a);
		~Color() = default;

		Color& operator=(const Color& p_other);

	private: 
		unsigned char m_r, m_g, m_b, m_a;
	};

}

