#include "Scene.h"
#include "Mesh.h"
#include "Entity.h"
#include "Light.h"

My::Scene::~Scene()
{
	m_entities.clear();

	for (const auto& pair : m_meshes)
		delete pair.second;
}

const My::Mesh* My::Scene::addMesh(const std::string& p_name, Mesh& p_mesh)
{
	if (m_meshes.find(p_name) != m_meshes.end())
		new (m_meshes[p_name]) Mesh(p_mesh);
	else
		m_meshes[p_name] = &p_mesh;

	return m_meshes[p_name];
}

const My::Mesh* My::Scene::getMesh(const std::string& p_name)
{
	if (m_meshes.find(p_name) == m_meshes.end())
		return nullptr;

	return m_meshes[p_name];
}

void My::Scene::addEntity(const Entity& p_entity)
{
	m_entities.push_back(p_entity);
}

std::vector<My::Entity> My::Scene::getEntities() const
{
	return m_entities;
}

My::Entity& My::Scene::getEntity(size_t index)
{
	if (index >= this->m_entities.size())
		throw "index out of range Scene.h l.44";

	return m_entities[index];
}
