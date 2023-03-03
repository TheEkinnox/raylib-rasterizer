#include "Light.h"
#include "Vertex.h"
#include "Trigonometry.h"
#include "Arithmetic.h"

My::Light::Light(Vec3 p_position, float p_ambientComponent, float p_diffuseComponent, float p_specularComponent)
{
	m_position = p_position;
	m_ambientComponent = LibMath::clamp(p_ambientComponent, 0, 1);
	m_diffuseComponent = LibMath::clamp(p_diffuseComponent, 0, 1);
	m_specularComponent = LibMath::clamp(p_specularComponent, 0, 1);

	m_intensity = Vec3(1); //how much rgb light it emits [0,1]
}

My::Color My::Light::CalculateLightingPhong(	const Vertex& p_vertex, const Vec3& p_observer,
												size_t p_shinyness)const
{
	// https://fr.wikipedia.org/wiki/Ombrage_de_Phong
	//float ka = 1, kd = 1, ks = 1; // material constants for percent of ambiante, diffuse and specular

	const float viewAngle = LibMath::clamp(	p_vertex.m_normal.dot(this->m_position - p_vertex.m_position), 
											0.0f, 1.0f);
	Vec3 halfVector = p_observer + this->m_position - 2 * p_vertex.m_position;
	halfVector.normalize();
	float nDotH = LibMath::clamp(p_vertex.m_normal.dot(halfVector), 0.0, 1.0); 

	// Intensity ambiante
	Vec3 ia = p_vertex.m_color.rgb() * m_ambientComponent;

	// Intensity diffuse
	Vec3 id = p_vertex.m_color.rgb() * m_diffuseComponent * nDotH;

	// Intensity specular
	Vec3 is = Color::white.rgb() * LibMath::pow(nDotH, static_cast<int>(p_shinyness)) * m_specularComponent;

	return Color(	ia + (id + is) * viewAngle * m_intensity,
					255);
}

//My::Color My::Light::ApplyLightToColor(const Color& p_color, float lightValue)
//{
//	return Color(	static_cast<uint8_t>(p_color.m_r * lightValue), static_cast<uint8_t>(p_color.m_g * lightValue),
//					static_cast<uint8_t>(p_color.m_b * lightValue), p_color.m_a);
//}
