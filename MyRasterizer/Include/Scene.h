#pragma once
#include <map>
#include <string>
#include <vector>
#include "Light.h"

namespace My
{
	/*
	Scene: représente la scène dessinée à l'écran 
	Elle contient les membres suivants :

	std::vector<Entity> entities

	La scène doit détruire tous les éléments qu’elle représente(meshes en l'occurrence) 
	lorsqu’elle est elle-même détruite. 
	*/

	class Mesh;
	class Entity;

	//struct Orientation
	//{
	//	LibMath::Vector3 right;
	//	LibMath::Vector3 up;
	//	LibMath::Vector3 front;
	//};

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
		void				addLight(const Light& p_light);

		std::vector<Entity>	getEntities() const;
		std::vector<Entity>&	getEntities();

		Entity&				getEntity(size_t index);
		std::vector<Light>	getLights() const;

		//const Orientation GlobalOrientation = {	LibMath::Vector3::right(),
		//										LibMath::Vector3::up(),
		//										LibMath::Vector3::front() };

	private:
		std::map<std::string, Mesh*> m_meshes;
		std::vector<Entity> m_entities;
		std::vector<Light> m_lights;
	};
}