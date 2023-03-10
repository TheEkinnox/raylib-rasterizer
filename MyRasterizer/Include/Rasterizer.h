#pragma once
#include <vector>
#include <functional>

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

		// TODO: Replace p_viewPos and p_mvpMatrix with camera
		typedef std::function<void(const Vertex p_vertices[3], Texture& p_target,
			const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
			const Mat4& p_mvpMatrix, const Texture* p_texture,
			std::vector<float>& p_zBuffer)> DrawFunc;

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
		 * \param p_zBuffer A reference to the rasterizer's zBuffer
		 */
		 // TODO: Replace p_viewPos and p_mvpMatrix with camera
		static void drawTriangleFill(const Vertex p_vertices[3], Texture& p_target,
			const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
			const Mat4& p_mvpMatrix, const Texture* p_texture,
			std::vector<float>& p_zBuffer);

		/**
		 * \brief Draws the received triangle's edges on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 * \param p_zBuffer A reference to the rasterizer's zBuffer
		 */
		static void drawTriangleWireFrame(const Vertex p_vertices[3], Texture& p_target,
			const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
			const Mat4& p_mvpMatrix, const Texture* p_texture,
			std::vector<float>& p_zBuffer);

		void toggleWireFrameMode();


	private:
		std::vector<float> m_zBuffer;

		enum class e_drawMode
		{
			E_FILL,
			E_WIRE_FRAME
		};

		e_drawMode m_drawMode = e_drawMode::E_FILL;
		DrawFunc m_drawTriangle = &Rasterizer::drawTriangleFill;

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

		static LibMath::Vector2 pointOnTriangleEdge(LibMath::Vector2 p_point, const LibMath::Vector2& p_triangleP1,
			const LibMath::Vector2& p_triangleP2);

		static bool fillCanDrawPixel(const LibMath::Vector3&);

		static bool wireFrameCanDrawPixel(	const LibMath::Vector2& p_pixelPos, const LibMath::Vector2 p_trianglePoints[3],
											const LibMath::Vector3& p_stw);
	};
}
