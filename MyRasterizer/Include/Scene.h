#pragma once
#include <vector>
#include "Entity.h"

namespace My
{
	class Mesh;
	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene& p_other) = default;
		Scene(Scene&& p_other) = default;
		~Scene();

		Scene& operator=(const Scene& p_other) = default;
		Scene& operator=(Scene&& p_other) = default;

	private:
		std::vector<Mesh*> m_meshes;
		std::vector<Entity> m_entities;
	};
}