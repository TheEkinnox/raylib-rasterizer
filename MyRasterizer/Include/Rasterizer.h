#pragma once
#include "Entity.h"
#include "Vector/Vector4.h"
#include "Matrix/Matrix4.h"

namespace My
{
	/*
	Cette fonction dessine la sc�ne en 3D dans la texture target.
	Elle doit d�abord remplir la texture target de noir.
	Elle doit ensuite rendre chaque entit� dans la texture en dessinant
	avec des lignes chaque triangle transform� un par un :
	chaque vertex est transform� par la matrice de transformation de l'entit�.
	*/

	struct Vertex;
	class Texture;
	class Scene;

	class Rasterizer
	{
		using Vec4 = LibMath::Vector4;
		using Mat4 = LibMath::Matrix4;

	public:
		/**
		 * \brief Draws the scene in 3D on the target texture
		 * \param p_scene The scene to render
		 * \param p_target The texture on which the scene should be rendered
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to view space
		 */
		static void renderScene(const Scene& p_scene, Texture& p_target,
			const Mat4& p_projectionMatrix);

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to view space
		 */
		static void drawEntity(const Entity& p_entity, Texture& p_target,
			const Mat4& p_projectionMatrix);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to view space
		 */
		static void drawTriangle(const Vertex p_vertices[3], Texture& p_target,
			const Mat4& p_projectionMatrix);

		/**
		 * \brief Converts the given world point to pixel coordinates
		 * \param p_pos The world position to convert
		 * \param p_target The target texture
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to view space
		 * \return The received world coordinates converted to pixel coordinates
		 */
		static Vec4 worldToPixel(const Vec4& p_pos, const Texture& p_target,
			const Mat4& p_projectionMatrix);
	};
}
