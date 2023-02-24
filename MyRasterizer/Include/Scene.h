#pragma once
#include <map>
#include <string>
#include <vector>

namespace My
{
	class Mesh;
	class Entity;

	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene& p_other) = default;
		Scene(Scene&& p_other) = default;
		~Scene();

		Scene&				operator=(const Scene& p_other) = default;
		Scene&				operator=(Scene&& p_other) = default;

		const Mesh*			addMesh(const std::string& p_name, Mesh& p_mesh);
		const Mesh*			getMesh(const std::string& p_name);

		void				addEntity(const Entity& p_entity);
		std::vector<Entity>	getEntities() const;

	private:
		std::map<std::string, Mesh*> m_meshes;
		std::vector<Entity> m_entities;
	};
}