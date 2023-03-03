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
		/// <summary>
		/// Light source to illumanate entites.
		/// components are clamped [0,1]
		/// </summary>
		Light(Vec3 p_position, float p_ambientComponent, float p_diffuseComponent, float p_specularComponent);
		~Light() = default;

		Color CalculateLightingPhong(const Vertex& p_vertex, const Vec3& p_observer, size_t p_shinyness = 10)const;

		//Color static ApplyLightToColor(const Color& p_color, float lightValue);

	private:
		Vec3 m_position;
		float m_ambientComponent;
		float m_diffuseComponent;
		float m_specularComponent;
		Vec3 m_intensity; //rgb light color
	};
}
