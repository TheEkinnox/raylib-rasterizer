#pragma once

namespace My
{
	/*
	Entity: représente une instance de mesh dessinée à l'écran 
	On peut donc avoir le même mesh dessiné plusieurs fois à l'écran 
	sans dupliquer les vertex et index buffer du mesh
	L'entité contient donc les membres suivants : 
	
	Mesh * mesh : pointeur sur le mesh
	Mat4 transform : transformation world(rotation, position, scale) de l'entité 
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

