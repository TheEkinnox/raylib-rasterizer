#include "Color.h"

My::Color::Color(unsigned char p_r, unsigned char p_g, unsigned char p_b, unsigned char p_a)
{
	this->m_r = p_r;
	this->m_g = p_g;
	this->m_b = p_b;
	this->m_a = p_a;
}

My::Color& My::Color::operator=(const Color& p_other)
{
	this->m_r = p_other.m_r;
	this->m_g = p_other.m_g;
	this->m_b = p_other.m_b;
	this->m_a = p_other.m_a;

	return *this;
}
