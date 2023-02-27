#include "Trigonometry.h"
#include "Mesh.h"

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

My::Mesh* My::Mesh::createCube(const Color& p_color)
{
	const std::vector<Vertex> vertices
	{
		{ { -.5f, .5f, .5f }, p_color },		// Front-top-left
		{ { .5f, .5f, .5f }, p_color },		// Front-top-right
		{ { -.5f, -.5f, .5f }, p_color },	// Front-bottom-left
		{ { .5f, -.5f, .5f }, p_color },		// Front-bottom-right
		{ { -.5f, .5f, -.5f }, p_color },	// Back-top-left
		{ { .5f, .5f, -.5f }, p_color },		// Back-top-right
		{ { -.5f, -.5f, -.5f }, p_color },	// Back-bottom-left
		{ { .5f, -.5f, -.5f }, p_color }		// Back-bottom-right
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

	return new Mesh(vertices, indices);
}

My::Mesh* My::Mesh::createSphere(const uint32_t p_latitudeCount, const uint32_t p_longitudeCount,
	const Color& p_color)
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

			const LibMath::Vector3 position
			{
				sinPhi * sinTheta * radius,
				cosPhi * radius,
				sinPhi * cosTheta * radius
			};

			vertices.push_back({ position, p_color });
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
