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

My::Scene& My::Scene::operator=(Scene&& p_other) noexcept
{
	if (&p_other == this)
		return *this;

	m_entities = std::move(p_other.m_entities);
	m_lights = std::move(p_other.m_lights);
	m_meshes = std::move(p_other.m_meshes);

	return *this;
}

const My::Mesh* My::Scene::addMesh(const std::string& p_name, Mesh& p_mesh)
{
	if (m_meshes.find(p_name) != m_meshes.end())
		delete m_meshes[p_name];

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

void My::Scene::addLight(const Light& p_light)
{
	m_lights.push_back(p_light);
}

std::vector<My::Entity> My::Scene::getEntities() const
{
	return m_entities;
}

std::vector<My::Entity>& My::Scene::getEntities()
{
	return m_entities;
}

My::Entity& My::Scene::getEntity(size_t index)
{
	if (index >= this->m_entities.size())
		throw "index out of range Scene.h l.44";

	return m_entities[index];
}

std::vector<My::Light> My::Scene::getLights() const
{
	return m_lights;
}
