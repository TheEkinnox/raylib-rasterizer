#include "Trigonometry.h"
#include "Mesh.h"
#include "Vector/Vector3.h"
#include "Arithmetic.h"
#include <map>
#include <set>
#include <numeric>

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

My::Mesh* My::Mesh::createCube(const Color& p_color)
{
	const std::vector<Vertex> vertices
	{
		//up
		{ { -.5f, .5f, -.5f }, Vec3::up(), p_color},		// Back-top-left
		{ { .5f, .5f, -.5f }, Vec3::up(), p_color  },		// Back-top-right
		{ { -.5f, .5f, .5f }, Vec3::up(), p_color  },		// Front-top-left
		{ { .5f, .5f, .5f }, Vec3::up(), p_color  },		// Front-top-right
		//front
		{ { -.5f, .5f, .5f }, Vec3::front(), p_color},		// Front-top-left
		{ { .5f, .5f, .5f }, Vec3::front(), p_color  },		// Front-top-right
		{ { -.5f, -.5f, .5f }, Vec3::front(), p_color  },	// Front-bottom-left
		{ { .5f, -.5f, .5f }, Vec3::front(), p_color  },	// Front-bottom-right
		//right
		{ { .5f, .5f, .5f }, Vec3::right(), p_color  },		// Front-top-right
		{ { .5f, .5f, -.5f }, Vec3::right(), p_color  },	// Back-top-right
		{ { .5f, -.5f, .5f }, Vec3::right(), p_color  },	// Front-bottom-right
		{ { .5f, -.5f, -.5f }, Vec3::right(), p_color  },	// Back-bottom-right
		//back
		{ { -.5f, .5f, -.5f }, Vec3::back(), p_color},		// Back-top-left
		{ { .5f, .5f, -.5f }, Vec3::back(), p_color  },		// Back-top-right
		{ { -.5f, -.5f, -.5f }, Vec3::back(), p_color  },	// Back-bottom-left
		{ { .5f, -.5f, -.5f }, Vec3::back(), p_color  },	// Back-bottom-right
		//left
		{ { -.5f, .5f, -.5f }, Vec3::left(), p_color},		// Back-top-left
		{ { -.5f, .5f, .5f }, Vec3::left(), p_color},		// Front-top-left
		{ { -.5f, -.5f, -.5f }, Vec3::left(), p_color  },	// Back-bottom-left
		{ { -.5f, -.5f, .5f }, Vec3::left(), p_color  },	// Front-bottom-left
		//down
		{ { -.5f, -.5f, .5f }, Vec3::down(), p_color  },	// Front-bottom-left
		{ { .5f, -.5f, .5f }, Vec3::down(), p_color  },		// Front-bottom-right
		{ { -.5f, -.5f, -.5f }, Vec3::down(), p_color  },	// Back-bottom-left
		{ { .5f, -.5f, -.5f }, Vec3::down(), p_color  }		// Back-bottom-right
	};

	std::vector<size_t> indices(36); // 6 faces * 2 triangles * 3 points
	//std::vector<size_t> indices; // 6 faces * 2 triangles * 3 points

	for (size_t i = 0; i < vertices.size(); i += 4) //4 vertex is one face
	{
		//triangle 1
		indices.push_back(i);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
		//triangle 2
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
	}

	return new Mesh(vertices, indices);
}

My::Mesh* My::Mesh::createSphere(const uint32_t p_latitudeCount, const uint32_t p_longitudeCount,
	const Color& p_color)
{
	const float deltaPhi = LibMath::g_pi / static_cast<float>(p_latitudeCount);
	const float deltaTheta = LibMath::g_pi * 2.f / static_cast<float>(p_longitudeCount);

	std::vector<Vertex> vertices;
	std::vector<size_t> indices;

	vertices.push_back({ Vec3::up(), Vec3::up(), p_color }); //top vertex
	for (uint32_t i = 1; i < p_latitudeCount; i++) // [1, count) bcs need only 1 vertex at top and bot
	{
		const float phi = static_cast<float>(i) * deltaPhi;
		const float cosPhi = cosf(phi);
		const float sinPhi = sinf(phi);

		for (uint32_t j = 0; j < p_longitudeCount; j++) //dont nee <= bcs using vertex 
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
				
			vertices.push_back({ normal * radius, normal, p_color});
		}
	}
	vertices.push_back({ Vec3::down(), Vec3::down(), p_color }); //bot vertex

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
		indices.push_back(i0);
		indices.push_back(i1);
	}

	// add quads per stack / slice
	for (uint32_t j = 0; j < p_latitudeCount - 2; j++) //dit top and bot cones
	{
		const auto j0 = j * p_longitudeCount + 1; //+1 is top point
		const auto j1 = (j + 1) * p_longitudeCount + 1;

		for (uint32_t i = 0; i < p_longitudeCount - 1; i++) //all but last square
		{
			const auto i0 = j0 + i;
			const auto i1 = j0 + (i + 1) % p_longitudeCount;
			const auto i2 = j1 + (i + 1) % p_longitudeCount;
			const auto i3 = j1 + i;

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indices.push_back(i2);
			indices.push_back(i3);
			indices.push_back(i0);
		}
		//last square wraps back to first vertex
		indices.push_back(j0 + p_longitudeCount - 1);
		indices.push_back(j0);
		indices.push_back(j1);

		indices.push_back(j1);
		indices.push_back(j1 + p_longitudeCount - 1);
		indices.push_back(j0 + p_longitudeCount - 1);
	}

	//test normals
	/*auto m1 = new Mesh(vertices, indices);
	auto m2 = new Mesh(*m1);

	m2->CalculateNormals();

	for (auto& v : m1->m_vertices)
		std::cout << v.m_normal << std::endl;
	std::cout << "my normals" << std::endl;
	for (auto& v : m2->m_vertices)
		std::cout << v.m_normal << std::endl;

	for (size_t i = 0; i < m1->m_vertices.size(); i++)
	{
		if (!LibMath::floatEquals(	m1->m_vertices[i].m_normal.m_x,
									m2->m_vertices[i].m_normal.m_x))
		{
			float dif = LibMath::abs(m2->m_vertices[i].m_normal.m_x - m1->m_vertices[i].m_normal.m_x);
			int l = 0;
		}
		
	}*/

	return new Mesh(vertices, indices);
}

void My::Mesh::CalculateNormals()
{
	Vec3* A, *B, *C;
	Vec3 BC, BA, normal;

	struct Compare
	{
		bool operator()(const Vec3& lhs, const Vec3& rhs) const noexcept
		{
			if (LibMath::floatGreaterThan(lhs.m_x,rhs.m_x)) //if x is bigger
				return true;

			if (LibMath::floatGreaterThan(lhs.m_y, rhs.m_y) &&
				LibMath::floatEquals(lhs.m_x, rhs.m_x)) //if y bigger and x ==
				return true;

			if (LibMath::floatGreaterThan(lhs.m_z, rhs.m_z) &&
				LibMath::floatEquals(lhs.m_x, rhs.m_x) && 
				LibMath::floatEquals(lhs.m_y, rhs.m_y)) //if z bigger and x and y ==
				return true;

			return false; //if x y z is smaler
		}
	};
	std::set<Vec3, Compare> uniqueNormals;
	std::map<Vertex*, std::set<const Vec3*>> vertexUniqueNormals; //each Vertex has a its set of unique normals ptrs

	for (size_t i = 0; i < this->m_indices.size(); i += 3)
	{
		//triangle ABC
		A = &this->m_vertices[m_indices[i]].m_position;
		B = &this->m_vertices[m_indices[i + 1]].m_position;
		C = &this->m_vertices[m_indices[i + 2]].m_position;

		//create plane
		BC = *C - *B;
		BA = *A - *B;	

		//get normal
		normal = BC.cross(BA);
		normal.normalize();

		//insert in uniqueNormals, returns pair<it,bool> (bool isNewInsert is ignored)
		//it is ptr to either new normal or existant normal (no duplicates) 
		const Vec3* normalPtr = &(*uniqueNormals.insert(normal).first);

		vertexUniqueNormals[&m_vertices[m_indices[i]]].insert(normalPtr);
		vertexUniqueNormals[&m_vertices[m_indices[i + 1]]].insert(normalPtr);
		vertexUniqueNormals[&m_vertices[m_indices[i + 2]]].insert(normalPtr);
	}

	//accumulate operator : add all the Vec3 ptr's values;
	auto op = [](const Vec3& v1, const Vec3* v2) { return v1 + *v2; }; 

	for (auto& pair : vertexUniqueNormals)
	{
		pair.first->m_normal = std::accumulate(	pair.second.begin(), pair.second.end(), 
												Vec3::zero(), op);
		pair.first->m_normal.normalize();
	}
}
