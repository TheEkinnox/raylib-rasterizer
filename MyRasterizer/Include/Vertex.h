#pragma once
#include "Vector/Vector3.h"
#include "Color.h"

namespace My
{
	struct Vertex
	{
		LibMath::Vector3	m_position;
		Color				m_color;
		float				u;
		float				v;
	};
}
