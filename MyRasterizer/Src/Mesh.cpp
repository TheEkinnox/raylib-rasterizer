#include "Trigonometry.h"
#include "Mesh.h"
#include "Vector/Vector3.h"
#include <map>

using Vec3 = LibMath::Vector3;

My::Mesh::Mesh(const std::vector<Vertex>& p_vertices, const std::vector<size_t>& p_indices)
{
	// Make sure the index buffer is a set of triangles
	if (p_indices.size() % 3 != 0)
		throw Exceptions::InvalidIndexBuffer();

	// Make sure the received indices are valid
	for (auto& index : p_indices)
		if (index < 0 || index >= p_vertices.size())
			throw Exceptions::InvalidIndexBuffer("Index [" + std::to_string(index) +
				"] is not part of the vertex buffer");

	this->m_vertices = p_vertices;
	this->m_indices = p_indices;
}

std::vector<My::Vertex> My::Mesh::getVertices() const
{
	return m_vertices;
}

std::vector<size_t> My::Mesh::getIndices() const
{
	return m_indices;
}

My::Mesh* My::Mesh::createCube()
{
	float nLen = 0.57735027f;	// 1 = sqrt(3 * x2) => sqrt(1/3) = x => 0.57735027f = x
	const std::vector<Vertex> vertices
	{
		{ { -.5f, .5f, .5f }, { -nLen, nLen, nLen } },		// Front-top-left
		{ { .5f, .5f, .5f }, { nLen, nLen, nLen } },		// Front-top-right
		{ { -.5f, -.5f, .5f }, { -nLen, -nLen, nLen } },	// Front-bottom-left
		{ { .5f, -.5f, .5f }, { nLen, -nLen, nLen } },		// Front-bottom-right
		{ { -.5f, .5f, -.5f }, { -nLen, nLen, -nLen } },	// Back-top-left
		{ { .5f, .5f, -.5f }, { nLen, nLen, -nLen } },		// Back-top-right
		{ { -.5f, -.5f, -.5f }, { -nLen, -nLen, -nLen } },	// Back-bottom-left
		{ { .5f, -.5f, -.5f }, { nLen, -nLen, -nLen } }	// Back-bottom-right
	};

	const std::vector<size_t> indices
	{
		// Front
		1, 0, 2,
		2, 3, 1,

		// Bottom
		3, 2, 7,
		7, 2, 6,

		// Left
		6, 2, 0,
		6, 0, 4,

		// Back
		6, 4, 5,
		5, 7, 6,

		// Right
		7, 5, 3,
		3, 5, 1,

		// Top
		1, 5, 4,
		4, 0, 1
	};
	//normal test
	//bool test = vertices[0].m_normal.isUnitVector();
	//Mesh* m = new Mesh(vertices, indices);
	//m->CalculateNormals();

	//return new Mesh(vertices, indices);

	return new Mesh(vertices, indices);
}

My::Mesh* My::Mesh::createSphere(const uint32_t p_latitudeCount, const uint32_t p_longitudeCount)
{
	const float deltaPhi = LibMath::g_pi / static_cast<float>(p_latitudeCount);
	const float deltaTheta = LibMath::g_pi * 2.f / static_cast<float>(p_longitudeCount);

	std::vector<Vertex> vertices;
	std::vector<size_t> indices;

	for (uint32_t i = 0; i < p_latitudeCount; i++)
	{
		const float phi = static_cast<float>(i) * deltaPhi;
		const float cosPhi = cosf(phi);
		const float sinPhi = sinf(phi);

		for (uint32_t j = 0; j <= p_longitudeCount; j++)
		{
			constexpr float radius = 1.f;
			const float theta = static_cast<float>(j) * deltaTheta;
			const float cosTheta = cosf(theta);
			const float sinTheta = -sinf(theta);

			const LibMath::Vector3 normal
			{
				sinPhi * sinTheta,
				cosPhi,
				sinPhi * cosTheta
			};

			bool test = normal.isUnitVector();
			float mag = normal.magnitudeSquared();
				
			vertices.push_back({ normal * radius, normal });
		}
	}

	for (uint32_t i = 0; i < p_longitudeCount; ++i)
	{
		// Top triangles
		auto i0 = i + 1;
		auto i1 = (i + 1) % p_longitudeCount + 1;
		indices.push_back(0);
		indices.push_back(i1);
		indices.push_back(i0);

		// Bottom triangles
		i0 = i + p_longitudeCount * (p_latitudeCount - 2) + 1;
		i1 = (i + 1) % p_longitudeCount + p_longitudeCount * (p_latitudeCount - 2) + 1;
		indices.push_back(vertices.size() - 1);
		indices.push_back(i1);
		indices.push_back(i0);
	}

	// add quads per stack / slice
	for (uint32_t j = 0; j < p_latitudeCount; j++)
	{
		const auto j0 = j * p_longitudeCount;
		const auto j1 = (j + 1) * p_longitudeCount;

		for (uint32_t i = 0; i < p_longitudeCount; i++)
		{
			const auto i0 = j0 + i;
			const auto i1 = j0 + (i + 1) % p_longitudeCount;
			const auto i2 = j1 + (i + 1) % p_longitudeCount;
			const auto i3 = j1 + i;

			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			indices.push_back(i2);
			indices.push_back(i0);
			indices.push_back(i3);
		}
	}

	return new Mesh(vertices, indices);
}

void My::Mesh::CalculateNormals()
{
	Vec3* A, *B, *C;
	Vec3 BC, BA, normal;

	for (size_t i = 0; i < this->m_indices.size() / 3; i += 3)
	{
		//triangle ABC
		A = &this->m_vertices[m_indices[i]].m_position;
		B = &this->m_vertices[m_indices[i + 1]].m_position;
		C = &this->m_vertices[m_indices[i + 2]].m_position;

		//create plane
		BC = *C - *B;
		BA = *A - *B;	

		/**
		* 
		*	|	 i		 j		 k	  |
		*	|	BC.x	BC.y	BC.z  |
		*	|	BA.x	BA.y	BA.z  |
		* 
		*/
		//get normal
		normal = {	BC.m_y * BA.m_z - BC.m_z * BA.m_y,
				  -(BC.m_x * BA.m_z - BC.m_z * BA.m_x),
					BC.m_x * BA.m_y - BC.m_y * BA.m_x };
		normal.normalize();

		//add normals to vertex
		m_vertices[m_indices[i]].m_normal += normal;
		m_vertices[m_indices[i + 1]].m_normal += normal;
		m_vertices[m_indices[i + 2]].m_normal += normal;
	}

	for (auto& vertex : m_vertices) //normalize all normals
	{
		if (vertex.m_normal != Vec3::zero())
			vertex.m_normal.normalize();
	}
}
