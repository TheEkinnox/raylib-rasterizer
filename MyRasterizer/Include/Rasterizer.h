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
	namespace Exceptions
	{
		class InvalidDrawMode : public std::exception
		{
		public:
			/**
			 * \brief Creates an invalid draw mode exception with a default message
			 */
			InvalidDrawMode() :
				std::exception("Divide By Zero") {}

			/**
			 * \brief Creates an invalid draw mode exception with a given message
			 * \param message The message of the exception
			 */
			InvalidDrawMode(char const* message) :
				std::exception(message) {}

			/**
			 * \brief Creates an invalid draw mode exception with a given message
			 * \param message The message of the exception
			 */
			InvalidDrawMode(const std::string& message) :
				std::exception(message.c_str()) {}
		};
	}

	class Camera;
	struct Vertex;
	class Texture;
	class Scene;
	class Vector3;

	class Rasterizer
	{
		enum class EDrawMode
		{
			E_FILL,
			E_WIRE_FRAME
		};

		using Vec3 = LibMath::Vector3;
		using Vec4 = LibMath::Vector4;
		using Mat4 = LibMath::Matrix4;
		
		typedef std::function<void(const Vertex p_vertices[3], const Texture* p_texture,
			Rasterizer& self)> DrawFunc;

	public:

		Rasterizer() = default;

		explicit Rasterizer(uint8_t p_sampleCount);

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
		 * \param p_camera The matrix to use to project points
		 * from world space to clip space
		 */
		void renderScene(const Scene& p_scene, const Camera& p_camera,
			Texture& p_target);

		void toggleWireFrameMode();


	private:
		std::vector<float>			m_zBuffer;
		const std::vector<Light>*	m_lights = nullptr;
		const Camera*				m_camera = nullptr;
		Texture*					m_target = nullptr;
		uint8_t						m_sampleCount = 1;
		EDrawMode					m_drawMode = EDrawMode::E_FILL;
		DrawFunc					m_drawTriangle = &Rasterizer::drawTriangleFill;

		/**
		 * \brief Draws the received entity on the target texture
		 * \param p_entity The entity to draw
		 */
		void drawEntity(const Entity& p_entity);

		/**
		 * \brief Draws the entity's normals on the target texture
		 * \param p_entity The entity to draw
		 */
		void drawNormals(const Entity& p_entity);

		/**
		 * \brief Draws the received triangle on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_texture The triangle's source texture
		 * \param p_self A reference to the rasterizer calling this function
		 */
		static void drawTriangleFill(const Vertex p_vertices[3], const Texture* p_texture,
			Rasterizer& p_self);

		/**
		 * \brief Draws the received triangle's edges on the target texture
		 * \param p_vertices The triangle to draw
		 * \param p_texture The triangle's source texture
		 * \param p_self A reference to the rasterizer calling this function
		 */
		static void drawTriangleWireFrame(const Vertex p_vertices[3], const Texture* p_texture,
			Rasterizer& p_self);

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
		
		/// <summary>
		/// Checks if triangle should be rendered based on normal
		/// </summary>
		/// <param name="p_trianglePos">: triangle's position</param>
		/// <param name="p_triangleNormal">: direction that the triangle is facing</param>
		/// <param name="p_viewPos">: observer's position</param>
		/// <returns></returns>
		static bool shouldDrawFace(	const Vec3& p_trianglePos, const Vec3& p_triangleNormal,
			const Vec3& p_viewPos);

		/// <summary>
		/// Checks if triangle should be rendered based on the observer's view direction
		/// </summary>
		/// <param name="p_trianglePos">: triangle's position</param>
		/// <param name="p_observerPos">: observer's position</param>
		/// <param name="p_observerDir">: direction towards which the observer looks</param>
		/// <returns></returns>
		static bool checkFacingDirection(const Vec3& p_trianglePos, const Vec3& p_observerPos,
			const Vec3& p_observerDir);
	};
}
