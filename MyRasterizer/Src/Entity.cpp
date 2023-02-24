#include "Entity.h"

#include <utility>

My::Entity::Entity(const Mesh& p_mesh, Mat4 p_transform) :
	m_mesh(&p_mesh), m_transform(std::move(p_transform))
{
	this->m_mesh = &p_mesh;
}

const My::Mesh* My::Entity::getMesh() const
{
	return m_mesh;
}

My::Entity::Mat4 My::Entity::getTransform() const
{
	return m_transform;
}

void My::Entity::setTransform(const Mat4& p_transform)
{
	m_transform = p_transform;
}
