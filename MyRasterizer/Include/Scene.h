#pragma once
#include <vector>
#include "Entity.h"

namespace My
{
	/*
	Scene: représente la scène dessinée à l'écran 
	Elle contient les membres suivants :

	std::vector<Entity> entities

	La scène doit détruire tous les éléments qu’elle représente(meshes en l'occurrence) 
	lorsqu’elle est elle-même détruite. 
	*/


	class Scene
	{
	public:
		Scene() = default;
		~Scene();

	private:
		std::vector<Entity> m_entities;

	};
}