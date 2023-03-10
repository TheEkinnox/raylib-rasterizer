#include "Entity.h"

#include <algorithm>

#include "Arithmetic.h"
#include "Mesh.h"

My::Entity::Entity(const Mesh& p_mesh, const Mat4& p_transform) :
	ITransformable(p_transform), m_mesh(&p_mesh), m_transparency(1.0f)
{
	this->m_mesh = &p_mesh;
}

My::Entity::Entity(const Mesh& p_mesh, const float p_transparency, const Mat4& p_transform) :
	ITransformable(p_transform), m_mesh(&p_mesh),
	m_transparency(LibMath::clamp(p_transparency, 0, 1))
{
}

const My::Mesh* My::Entity::getMesh() const
{
	return m_mesh;
}

float My::Entity::getTransparency() const
{
	return m_transparency;
}

bool My::Entity::isOpaque() const
{
	if (!LibMath::floatEquals(m_transparency, 1.f))
		return false;

	const auto vertices = m_mesh->getVertices();

	return !std::all_of(vertices.begin(), vertices.end(),
		[](const Vertex& vertex) { return vertex.m_color.m_a != UINT8_MAX; });
}
