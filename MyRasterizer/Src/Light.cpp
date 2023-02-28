#include "Light.h"
#include "Vertex.h"
#include "Trigonometry.h"
#include "Arithmetic.h"

float My::Light::CalculateLightingPhong(	const Vertex& p_vertex, const Vec3& p_observer, 
											size_t p_shinyness = 10,	Vec3 p_matCaracPercent = Vec3(1, 1, 1))
{
	// https://fr.wikipedia.org/wiki/Ombrage_de_Phong
	//float ka = 1, kd = 1, ks = 1; // material constants for percent of ambiante, diffuse and specular

	float Ia, Id, Is;		// Intensity ambiante, diffuse and specular
	Vec3 L, N, V, R;		// light, normal, observer, reflexion
	Rad teta, omega;		// normal from light, observer from reflexion

	L = (this->m_position - p_vertex.m_position);
	N = p_vertex.m_normal;
	V = p_observer - p_vertex.m_position;

	L.normalize();
	N.normalize();
	V.normalize();

	teta =	N.angleFrom(L);
	R =		(2 * LibMath::cos(teta)) * (N - L);
	omega = V.angleFrom(R);

	Ia = (m_ambientComponent  * p_matCaracPercent.m_x);
	Id = (m_diffuseComponent  * p_matCaracPercent.m_y) * LibMath::cos(teta);
	Is = (m_specularComponent * p_matCaracPercent.m_z) * LibMath::pow(LibMath::cos(omega), p_shinyness);

	return Ia + Is + Id;
}

My::Color My::Light::ApplyLightToVertex(const Color& p_color, float lightValue)
{
	return Color(	static_cast<uint8_t>(p_color.m_r * lightValue), static_cast<uint8_t>(p_color.m_g * lightValue),
					static_cast<uint8_t>(p_color.m_b * lightValue), p_color.m_a);
}
