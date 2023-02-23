#pragma once

namespace My
{
	/*
	Rasterizer: contient votre pipeline de rendu
	Elle contient les membres suivants :

	void RenderScene(Scene * pScene, Texture * pTarget)

	Cette fonction dessine la sc�ne en 3D dans la texture target.
	Elle doit d�abord remplir la texture target de noir.
	Elle doit ensuite rendre chaque entit� dans la texture en dessinant 
	avec des lignes chaque triangle transform� un par un : 
	chaque vertex est transform� par la matrice de transformation de l'entit�. 
	*/

	class Rasterizer
	{
	public:
		Rasterizer();
		~Rasterizer();

	private:

	};
}