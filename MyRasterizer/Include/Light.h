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
		/// Light source to illuminate entities at p_position.
		/// Components are percents that must add to 1
		/// </summary>
		/// <param name="p_position"> pos in 3D space</param>
		/// <param name="p_ambientComponent">constant lighting on entities</param>
		/// <param name="p_diffuseComponent">spotlight lighting on entities</param>
		/// <param name="p_specularComponent">white spec on entities</param>
		/// <param name="p_intensity">The light's intensity</param>
		Light(const Vec3& p_position, float p_ambientComponent, float p_diffuseComponent,
			float p_specularComponent, float p_intensity);

		/// <summary>
		/// Light source to illuminate entities at p_position.
		/// Components are percents that must add to 1
		/// </summary>
		/// <param name="p_position"> pos in 3D space</param>
		/// <param name="p_ambientComponent">constant lighting on entities</param>
		/// <param name="p_diffuseComponent">spotlight lighting on entities</param>
		/// <param name="p_specularComponent">white spec on entities</param>
		/// <param name="p_intensity">The intensity of the light's red, green and blue components</param>
		Light(const Vec3& p_position, float p_ambientComponent, float p_diffuseComponent,
			float p_specularComponent, const Vec3& p_intensity);

		~Light() = default;

		/// <summary>
		/// Calculate the luminosity/color of a vertex based on the Phong method
		/// </summary>
		/// <param name="p_vertex"> target vertex</param>
		/// <param name="p_observer"> camera position</param>
		/// <param name="p_shininess"> material shininess constant </param>
		/// <returns></returns>
		Color calculateLightingPhong(const Vertex& p_vertex, const Vec3& p_observer, size_t p_shininess = 10) const;

		/**
		 * \brief Calculates the lit color of a vertex using the Blinn-Phong reflection model
		 * \param p_position the position at which the calculation should be done
		 * \param p_color the interpolated color to use
		 * \param p_normal the interpolated normal to use
		 * \param p_viewPos the position of the observer
		 * \param p_shininess the material's shininess
		 * \return The lit color of the received vertex
		 */
		Color calculateLightingBlinnPhong(const Vec3& p_position,
			const Color& p_color, const Vec3& p_normal, const Vec3& p_viewPos,
			int p_shininess = 20) const;

		//Color static ApplyLightToColor(const Color& p_color, float lightValue);

	private:
		Vec3 m_position;
		float m_ambientComponent;
		float m_diffuseComponent;
		float m_specularComponent;
		Vec3 m_intensity; //rgb light color from [0,1]
	};
}
