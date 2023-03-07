#include "Light.h"
#include "Vertex.h"
#include "Arithmetic.h"

My::Light::Light(const Vec3& p_position, const float p_ambientComponent,
	const float p_diffuseComponent, const float p_specularComponent,
	const float p_intensity) :
	Light(p_position, p_ambientComponent, p_diffuseComponent, p_specularComponent, Vec3(p_intensity))
{
}

My::Light::Light(const Vec3& p_position, const float p_ambientComponent,
	const float p_diffuseComponent, const float p_specularComponent,
	const Vec3& p_intensity) :
	m_position(p_position), m_ambientComponent(LibMath::clamp(p_ambientComponent, 0, 1)),
	m_diffuseComponent(LibMath::clamp(p_diffuseComponent, 0, 1)),
	m_specularComponent(LibMath::clamp(p_specularComponent, 0, 1)),
	m_intensity(p_intensity)
{
}

My::Color My::Light::calculateLightingPhong(const Vertex& p_vertex, const Vec3& p_observer,
                                            const size_t p_shininess) const
{
	// https://fr.wikipedia.org/wiki/Ombrage_de_Phong

	const float lightAngle = LibMath::clamp(	p_vertex.m_normal.dot(this->m_position - p_vertex.m_position), 
											0.0f, 1.0f);
	Vec3 halfVector = p_observer + this->m_position - 2 * p_vertex.m_position;
	halfVector.normalize();
	float nDotH = LibMath::clamp(p_vertex.m_normal.dot(halfVector), 0.0, 1.0); 

	// Intensity ambient
	const Vec3 ia = p_vertex.m_color.rgb() * m_ambientComponent;

	// Intensity diffuse
	const Vec3 id = p_vertex.m_color.rgb() * m_diffuseComponent * nDotH; // TODO : remove * nDotH

	// Intensity specular
	const Vec3 is = Color::white.rgb() * LibMath::pow(nDotH, static_cast<int>(p_shininess)) * m_specularComponent;

	return Color(	ia + (id + is) * lightAngle * m_intensity,
					255);
}

My::Color My::Light::calculateLightingBlinnPhong(const Vec3& p_position,
	const Color& p_color, const Vec3& p_normal, const Vec3& p_viewPos,
	const int p_shininess) const
{
	const Vec3 lightDir = (m_position - p_position).normalized();

	const float squaredDist = m_position.distanceSquaredFrom(p_position);

	const float lambertian = LibMath::max(lightDir.dot(p_normal), 0.f);
	float specular = 0.f;

	if (lambertian > 0.f)
	{
		const Vec3 viewDir = (p_viewPos - p_position).normalized();
		const Vec3 halfDir = (lightDir + viewDir).normalized();
		const float specularAngle = LibMath::max(halfDir.dot(p_normal), 0.f);

		if (LibMath::floatEquals(specularAngle, 0.f))
			int i = 0;

		specular = LibMath::pow(specularAngle, p_shininess);
	}

	const Color ambientColor = p_color * m_ambientComponent;
	const Color diffuseColor = p_color * m_diffuseComponent;
	const Color specularColor = p_color * m_specularComponent;

	Color color = ambientColor
		+ diffuseColor * lambertian * m_intensity / squaredDist
		+ specularColor * specular * m_intensity / squaredDist;

	color.m_a = p_color.m_a;

	return color;
}

//My::Color My::Light::ApplyLightToColor(const Color& p_color, float lightValue)
//{
//	return Color(	static_cast<uint8_t>(p_color.m_r * lightValue), static_cast<uint8_t>(p_color.m_g * lightValue),
//					static_cast<uint8_t>(p_color.m_b * lightValue), p_color.m_a);
//}
