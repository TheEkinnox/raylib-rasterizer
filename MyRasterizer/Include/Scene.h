#pragma once
#include <map>
#include <string>
#include <vector>

namespace My
{
	/*
	Scene: repr�sente la sc�ne dessin�e � l'�cran 
	Elle contient les membres suivants :

	std::vector<Entity> entities

	La sc�ne doit d�truire tous les �l�ments qu�elle repr�sente(meshes en l'occurrence) 
	lorsqu�elle est elle-m�me d�truite. 
	*/

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