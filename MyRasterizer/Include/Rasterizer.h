#pragma once
#include "Entity.h"

namespace My
{
	/*
	Cette fonction dessine la scène en 3D dans la texture target.
	Elle doit d’abord remplir la texture target de noir.
	Elle doit ensuite rendre chaque entité dans la texture en dessinant
	avec des lignes chaque triangle transformé un par un :
	chaque vertex est transformé par la matrice de transformation de l'entité.
	*/

	struct Vertex;
	class Texture;
	class Scene;

	class Rasterizer
	{
	public:
		/**
		 * \brief Draws the scene in 3D on the target texture
		 * \param p_scene The scene to render
		 * \param p_target The texture on which the scene should be rendered
		 */
		static void renderScene(const Scene& p_scene, Texture& p_target);

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 */
		static void drawEntity(const Entity& p_entity, Texture& p_target);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 */
		static void drawTriangle(const Vertex p_vertices[3], Texture& p_target, const Texture* p_texture);

	};
}