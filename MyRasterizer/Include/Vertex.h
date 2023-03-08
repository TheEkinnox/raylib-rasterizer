#pragma once
#include "Vector/Vector3.h"
#include "Color.h"

namespace My
{
	struct Vertex
	{
		LibMath::Vector3	m_position;
		LibMath::Vector3	m_normal;
		Color				m_color;
		float				m_u;
		float				m_v;
	};
}
