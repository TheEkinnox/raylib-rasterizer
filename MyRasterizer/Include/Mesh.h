#pragma once
#include <vector>
#include "Vertex.h"

namespace My
{
	namespace Exceptions
	{
		class InvalidIndexBuffer : public std::exception
		{
		public:
			/**
			 * \brief Creates an invalid index buffer exception with a default message
			 */
			InvalidIndexBuffer() :
				std::exception("Invalid index buffer") {}

			/**
			 * \brief Creates an invalid index buffer exception with a given message
			 * \param message The message of the exception
			 */
			InvalidIndexBuffer(char const* message) :
				std::exception(message) {}

			/**
			 * \brief Creates an invalid index buffer exception with a given message
			 * \param message The message of the exception
			 */
			InvalidIndexBuffer(const std::string& message) :
				std::exception(message.c_str()) {}
		};
	}

	class Mesh
	{
	public:
		/**
		 * \brief Creates a mesh with the given set of vertices and indices
		 * \brief Vertex color converted to opaque Color if transparent
		 * \param p_vertices The vertex buffer of the mesh
		 * \param p_indices The index buffer of the mesh
		 */
		Mesh(const std::vector<Vertex>& p_vertices, const std::vector<size_t>& p_indices);

		/**
		 * \brief Creates a copy of the given mesh
		 * \param p_other The mesh to copy
		 */
		Mesh(const Mesh& p_other) = default;

		/**
		 * \brief Creates a move copy of the given mesh
		 * \param p_other The mesh to move
		 */
		Mesh(Mesh&& p_other) = default;

		/**
		 * \brief Destroys the current mesh
		 */

		~Mesh() = default;

		/**
		 * \brief Copies the given mesh into the current one
		 * \param p_other The mesh to copy
		 */
		Mesh&	operator=(const Mesh& p_other) = default;

		/**
		 * \brief Moves the given mesh into the current one
		 * \param p_other The mesh to move
		 */
		Mesh&	operator=(Mesh&& p_other) = default;

		/**
		 * \brief Gives read access to the vertex buffer of the mesh
		 * \return The vertex buffer of the mesh
		 */
		std::vector<Vertex>	getVertices() const;

		/**ss
		 * \brief Gives read access to the index buffer of the mesh
		 * \return The index buffer of the mesh
		 */
		std::vector<size_t>	getIndices() const;

		/**
		* \brief Calculates the normal of each Vertex
		*/
		void calculateNormals();

		/**
		 * \brief Creates a cube of side 1
		 * \param p_color The color of the cube (white by default)
		 * \return A pointer to the created cube mesh
		 */
		static Mesh* createCube(Color p_color = Color::white);

		/**
		 * \brief Creates a sphere of radius 1 with the given number of subdivisions
		 * \param p_latitudeCount The number of vertical subdivisions
		 * \param p_longitudeCount The number of horizontal subdivisions
		 * \param p_color The color of the sphere (white by default)
		 * \return A pointer to the created sphere mesh
		 */
		static Mesh* createSphere(	uint32_t p_latitudeCount, uint32_t p_longitudeCount,
									Color p_color = Color::white);

	private:
		std::vector<Vertex> m_vertices;
		std::vector<size_t> m_indices;
	};

}