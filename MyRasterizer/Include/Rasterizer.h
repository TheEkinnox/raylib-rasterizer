#pragma once
#include <vector>

#include "Entity.h"
#include "Light.h"

namespace My
{
	struct Vertex;
	class Texture;
	class Scene;
	class Vector3;

	class Rasterizer
	{
		using Vec3 = LibMath::Vector3;
	public:
		Rasterizer() = default;

		/**
		 * \brief Creates a move copy of the given rasterizer
		 * \param p_other The rasterizer to move
		 */
		Rasterizer(const Rasterizer& p_other) = default;

		/**
		 * \brief Creates a move copy of the given rasterizer
		 * \param p_other The rasterizer to move
		 */
		Rasterizer(Rasterizer&& p_other) = default;
		~Rasterizer() = default;

		/**
		 * \brief Copies the given rasterizer into the current one
		 * \param p_other The rasterizer to copy
		 */
		Rasterizer&			operator=(const Rasterizer& p_other) = default;

		/**
		 * \brief Moves the given rasterizer into the current one
		 * \param p_other The rasterizer to move
		 */
		Rasterizer&			operator=(Rasterizer&& p_other) = default;

		/**
		 * \brief Draws the scene in 3D on the target texture
		 * \param p_scene The scene to render
		 * \param p_target The texture on which the scene should be rendered
		 */
		void				renderScene(const Scene& p_scene, Texture& p_target);

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 */
		void				drawEntity(const Entity& p_entity, Texture& p_target);

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 * \param 
		 */
		void drawEntity(const Entity& p_entity, Texture& p_target, const Light& p_light);

		/**
		 * \brief Draws the entity's normals on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 */
		void drawNormals(const Entity& p_entity, Texture& p_target, const Light& p_light);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 */
		void				drawTriangle(const Vertex p_vertices[3], Texture& p_target);

	private:
		std::vector<float>	m_zBuffer;
		
		/// <summary>
		/// Checks if triangle should be rendered
		/// </summary>
		/// <param name="p_trianglePos">: triangle's position</param>
		/// <param name="p_triangleNormal">: direction that the triangle is facing</param>
		/// <param name="p_observerPos">: observer's position</param>
		/// <param name="p_observerDir">: direction that the observer is loocking at</param>
		/// <returns></returns>
		bool checkBackFaceCulling(	const Vec3& p_trianglePos, const Vec3& p_triangleNormal, 
									const Vec3& p_observerPos, const Vec3& p_observerDir)const;
	};
}
