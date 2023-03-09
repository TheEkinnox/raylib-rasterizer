#pragma once
#include <vector>

#include "Entity.h"
#include "Light.h"
#include "Vector/Vector3.h"
#include "Vector/Vector4.h"
#include "Matrix/Matrix4.h"

namespace My
{
	struct Vertex;
	class Texture;
	class Scene;

	class Rasterizer
	{
		using Vec3 = LibMath::Vector3;
		using Vec4 = LibMath::Vector4;
		using Mat4 = LibMath::Matrix4;

	public:

		Rasterizer() : Rasterizer(1.0f)
		{}

		Rasterizer(float p_msaaValue);

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
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to clip space
		 */
		void renderScene(const Scene& p_scene, Texture& p_target,
			const Mat4& p_projectionMatrix);

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 * \param p_projectionMatrix The matrix to use to project points
		 * from world space to clip space
		 * \param p_lights The lights to apply to the entity
		 * \param p_viewPos The position from which the entity is seen
		 */
		void drawEntity(const Entity& p_entity, Texture& p_target,
			const Mat4& p_projectionMatrix, const std::vector<Light>& p_lights,
			const LibMath::Vector3& p_viewPos);

		/**
		 * \brief Draws the entity's normals on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 * \param p_viewPos The position from which the entity is seen
		 */
		void drawNormals(const Entity& p_entity, Texture& p_target,
			const LibMath::Vector3& p_viewPos);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 * \param p_lights The light used to shade the triangle
		 * \param p_viewPos The position from which the triangle is seen
		 * \param p_mvpMatrix The model view projection matrix used to convert the point
		 * from world space to clip space
		 * \param p_texture The texture of the triangle
		 */
		// TODO: Remove p_mvpMatrix and p_viewPos when camera is implemented
		void drawTriangle(const Vertex p_vertices[3], Texture& p_target,
			const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
			const Mat4& p_mvpMatrix, const Texture* p_texture);

		/**
		 * \brief Converts the given world point to pixel coordinates
		 * \param p_pos The world position to convert
		 * \param p_target The target texture
		 * \param p_mvpMatrix The model view projection matrix used to convert the point
		 * from world space to clip space
		 * \return The received world coordinates converted to pixel coordinates
		 */
		static Vec3 worldToPixel(const Vec3& p_pos, const Texture& p_target,
			const Mat4& p_mvpMatrix);

	private:
		std::vector<float>	m_zBuffer;
		float m_msaaValue;
		std::shared_ptr<Texture> m_msaaTexture;
	};
}
