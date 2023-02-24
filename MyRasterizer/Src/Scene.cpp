#include "Scene.h"
#include "Mesh.h"

My::Scene::~Scene()
{
	m_entities.clear();

	for (const auto* mesh : m_meshes)
		delete mesh;
}
