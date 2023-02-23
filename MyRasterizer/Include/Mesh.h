#pragma once
#include <vector>
#include "Vertex.h"

namespace My
{
	/*
	Mesh: contient les informations membres suivantes :

	std::vector<Vertex> vertices : le vertex buffer du mesh.
	Les vertices du mesh ne doivent jamais changer pendant la simulation
	Pour déplacer un objet, on modifie sa matrice de transformation, pas les vertices du mesh

	std::vector<int> indices : l’index buffer du mesh.
	C’est une suite de triplets d’index du vertex buffer qui déterminent les triangles du mesh.
	ex : 0, 1, 2 désigne le triangle(vertices[0], vertices[1], vertices[2])

	Le mesh doit aussi contenir les fonctions 

	static Mesh * CreateCube
	Crée un cube de côté 1
	Composé de 6 faces, donc 12 

	static Mesh * CreateSphere(int latitudeCount, int longitudeCount)
	Crée une sphère de rayon 1
	*/

	class Mesh
	{
	public:

		Mesh(const std::vector<Vertex>& p_vertices, const std::vector<int> p_indices);
		~Mesh() = default;

		static Mesh* CreateCube();
		static Mesh* CreateSphere(int latitudeCount, int longitudeCount);

	private:
		std::vector<Vertex> m_vertices;
		std::vector<int> m_indices;
	};

}