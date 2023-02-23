#pragma once

namespace My
{
	/*
	Rasterizer: contient votre pipeline de rendu
	Elle contient les membres suivants :

	void RenderScene(Scene * pScene, Texture * pTarget)

	Cette fonction dessine la scène en 3D dans la texture target.
	Elle doit d’abord remplir la texture target de noir.
	Elle doit ensuite rendre chaque entité dans la texture en dessinant 
	avec des lignes chaque triangle transformé un par un : 
	chaque vertex est transformé par la matrice de transformation de l'entité. 
	*/

	class Rasterizer
	{
	public:
		Rasterizer();
		~Rasterizer();

	private:

	};
}