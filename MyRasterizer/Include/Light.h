#pragma once
#include "Vector/Vector3.h"
#include "Angle/Radian.h"
#include "Color.h"

namespace My
{
	struct Vertex;

	class Light
	{
		// Syntactic sugar for shorter lines
		using Vec3 = LibMath::Vector3;
		using Rad =  LibMath::Radian;

	public: 
		Light() = default;
		~Light() = default;

		float CalculateLightingPhong(const Vertex& p_vertex, const Vec3& p_observer, size_t, Vec3);

		Color static ApplyLightToVertex(const Color& p_color, float lightValue);

	private:
		Vec3 m_position;
		float m_ambientComponent;
		float m_diffuseComponent;
		float m_specularComponent;
	};
}
