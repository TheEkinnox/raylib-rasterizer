#pragma once
#include <vector>
#include "Entity.h"

namespace My
{
	/*
	Scene: repr�sente la sc�ne dessin�e � l'�cran 
	Elle contient les membres suivants :

	std::vector<Entity> entities

	La sc�ne doit d�truire tous les �l�ments qu�elle repr�sente(meshes en l'occurrence) 
	lorsqu�elle est elle-m�me d�truite. 
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