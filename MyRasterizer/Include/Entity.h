#pragma once

namespace My
{
	/*
	Entity: repr�sente une instance de mesh dessin�e � l'�cran 
	On peut donc avoir le m�me mesh dessin� plusieurs fois � l'�cran 
	sans dupliquer les vertex et index buffer du mesh
	L'entit� contient donc les membres suivants : 
	
	Mesh * mesh : pointeur sur le mesh
	Mat4 transform : transformation world(rotation, position, scale) de l'entit� 
	*/

	class Mesh;
	class Mat4;

	class Entity
	{
		Entity(const Mesh& p_mesh);
		~Entity() = default;

	private:
		const Mesh* m_mesh;
		//Mat4 m_transform; //TODO: mat4
	};
}

