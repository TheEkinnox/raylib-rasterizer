#pragma once
#include <vector>

#include "Entity.h"
#include "Light.h"
#include <functional>

namespace My
{
	struct Vertex;
	class Texture;
	class Scene;

	typedef std::function<void(const Vertex p_vertices[3], Texture& p_target, std::vector<float>& p_zBuffer)> DrawFunc;

	class Rasterizer
	{
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
		 * \param p_light The light source illuminating the drawn entity
		 * \param p_draw The triangle drawing function to use
		 */
		void drawEntity(const Entity& p_entity, Texture& p_target, const Light& p_light, DrawFunc p_draw);

		/**
		 * \brief Draws the entity's normals on the target texture
		 * \param p_entity The entity to draw
		 * \param p_target The texture on which the entity should be drawn
		 * \param p_light The light source illuminating the drawn entity
		 */
		void drawNormals(const Entity& p_entity, Texture& p_target, const Light& p_light);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 * \param p_zBuffer A reference to the rasterizer's zBuffer
		 */
		static void drawTriangleFill(		const Vertex p_vertices[3], Texture& p_target, std::vector<float>& p_zBuffer);

		/**
		 * \brief Draws the received triangle's edges on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_target The texture on which the triangle should be drawn
		 * \param p_zBuffer A reference to the rasterizer's zBuffer
		 */
		static void drawTriangleWireFrame(	const Vertex p_vertices[3], Texture& p_target, std::vector<float>& p_zBuffer);

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

		static LibMath::Vector2 pointOnTriangleEdge(	LibMath::Vector2 p_point, const LibMath::Vector2& p_triangleP1,
														const LibMath::Vector2& p_triangleP2);

		static bool wireFrameCanDrawPixel(	const LibMath::Vector2& p_pixelPos, const LibMath::Vector2 p_trianglePoints[3],
											const LibMath::Vector3&);
		static bool fillCanDrawPixel( const LibMath::Vector3&);
	};
}
