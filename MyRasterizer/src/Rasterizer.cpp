#include "Rasterizer.h"

#include "Arithmetic.h"
#include "Color.h"
#include "Mesh.h"
#include "Texture.h"
#include "Scene.h"
#include "Light.h"
#include "Vector/Vector2.h"
#include "Vector/Vector4.h"
#include <Interpolation.h>

namespace My
{
	void Rasterizer::renderScene(const Scene& p_scene, Texture& p_target)
	{
		// Set every pixel to black
		for (uint32_t x = 0; x < p_target.getWidth(); x++)
			for (uint32_t y = 0; y < p_target.getHeight(); y++)
				p_target.setPixelColor(x, y, Color::black);

		const size_t textureSize = static_cast<size_t>(p_target.getWidth())
			* p_target.getHeight();

		m_zBuffer.clear();
		m_zBuffer.resize(textureSize, INFINITY);

		for (const auto& entity : p_scene.getEntities())
		{
			drawEntity(entity, p_target, p_scene.getLights()[0]);
			//drawNormals(entity, p_target);
		}
	}

	void Rasterizer::drawEntity(const Entity& p_entity, Texture& p_target)
	{
		if (p_entity.getMesh() != nullptr)
		{
			const auto vertices = p_entity.getMesh()->getVertices();
			const auto indices = p_entity.getMesh()->getIndices();

			for (size_t i = 0; i + 2 < indices.size(); i += 3)
			{
				Vertex triangle[3]
				{
					vertices[indices[i]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				};

				for (Vertex& vertex : triangle)
				{
					auto& pos = vertex.m_position;
					auto vec4 = LibMath::Vector4{pos.m_x, pos.m_y, pos.m_z, 1.f};

					vec4 = p_entity.getTransform() * vec4;
					pos = {vec4.m_x, vec4.m_y, vec4.m_z};
				}

				drawTriangle(triangle, p_target);
			}
		}
	}

	void Rasterizer::drawEntity(const Entity& p_entity, Texture& p_target, const Light& p_light)
	{
		if (p_entity.getMesh() != nullptr)
		{
			const auto vertices = p_entity.getMesh()->getVertices();
			const auto indices = p_entity.getMesh()->getIndices();

			std::map<const Vertex*, Color> vertexLightColor;

			//calculate light
			for (const Vertex& vertex : vertices)
			{
				Vertex v = vertex;

				{
					auto& pos = v.m_position; //update pos
					auto vec4 = LibMath::Vector4{ pos.m_x, pos.m_y, pos.m_z, 1.f };
					vec4 = p_entity.getTransform() * vec4;
					pos = { vec4.m_x, vec4.m_y, vec4.m_z };
				}

				{
					auto& nor = v.m_normal; //update normal
					auto vec4 = LibMath::Vector4{ nor.m_x, nor.m_y, nor.m_z, 1.f };
					vec4 = p_entity.getRotation() * vec4;
					nor = { vec4.m_x, vec4.m_y, vec4.m_z };
				}

				vertexLightColor[&vertex] = p_light.CalculateLightingPhong(v, LibMath::Vector3::zero());
			}

			for (size_t i = 0; i + 2 < indices.size(); i += 3)
			{
				Vertex triangle[3]
				{
					vertices[indices[i]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				};

				triangle[0].m_color = vertexLightColor[&vertices[indices[i]]];
				triangle[1].m_color = vertexLightColor[&vertices[indices[i + 1]]];
				triangle[2].m_color = vertexLightColor[&vertices[indices[i + 2]]];

				for (Vertex& vertex : triangle)
				{
					auto& pos = vertex.m_position;
					auto vec4 = LibMath::Vector4{ pos.m_x, pos.m_y, pos.m_z, 1.f };

					vec4 = p_entity.getTransform() * vec4;
					pos = { vec4.m_x, vec4.m_y, vec4.m_z };
				}

				drawTriangle(triangle, p_target);
			}
		}
	}

	void Rasterizer::drawNormals(const Entity& p_entity, Texture& p_target)
	{
		if (p_entity.getMesh() != nullptr)
		{
			const auto vertices = p_entity.getMesh()->getVertices();
			const auto indices = p_entity.getMesh()->getIndices();

			for (auto v : vertices)
			{

				{
					auto& pos = v.m_position; //update pos
					auto vec4 = LibMath::Vector4{ pos.m_x, pos.m_y, pos.m_z, 1.f };
					vec4 = p_entity.getTransform() * vec4;
					pos = { vec4.m_x, vec4.m_y, vec4.m_z };
				}

				{
					auto& nor = v.m_normal; //update normal
					auto vec4 = LibMath::Vector4{ nor.m_x, nor.m_y, nor.m_z, 1.f };
					vec4 = p_entity.getRotation() * vec4;
					nor = { vec4.m_x, vec4.m_y, vec4.m_z };
				}

				Vertex triangle1[3]
				{
					Vertex{v.m_position + LibMath::Vector3(0.02f), LibMath::Vector3::zero(), Color::white},
					Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),Color::white},
					Vertex{v.m_position,LibMath::Vector3::zero(),Color::white}
				};

				drawTriangle(triangle1, p_target);

				Vertex triangle2[3]
				{
					Vertex{v.m_position + LibMath::Vector3(0.02f),LibMath::Vector3::zero(),Color::white},
					Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),Color::white},
					Vertex{v.m_position + v.m_normal + LibMath::Vector3(0.02f),LibMath::Vector3::zero(),Color::white }
				};

				drawTriangle(triangle2, p_target);
			}
		}
	}

	void Rasterizer::drawTriangle(const Vertex p_vertices[3], Texture& p_target)
	{
		// Create an array of vector4 for the positions
		LibMath::Vector4 points[3]
		{
			{p_vertices[0].m_position.m_x, p_vertices[0].m_position.m_y, p_vertices[0].m_position.m_z, 1.f},
			{p_vertices[1].m_position.m_x, p_vertices[1].m_position.m_y, p_vertices[1].m_position.m_z, 1.f},
			{p_vertices[2].m_position.m_x, p_vertices[2].m_position.m_y, p_vertices[2].m_position.m_z, 1.f}
		};

		// Create our projection matrix
		LibMath::Matrix4 projMat;
		projMat[projMat.getIndex(0, 0)] = .2f;
		projMat[projMat.getIndex(0, 3)] = 1.f;
		projMat[projMat.getIndex(1, 1)] = .2f;
		projMat[projMat.getIndex(1, 3)] = 1.f;
		projMat[projMat.getIndex(3, 3)] = 1.f;

		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		// Project each point on the screen
		for (LibMath::Vector4& pos : points)
		{
			pos = projMat * pos;

			pos.m_x *= 0.5f * floatWidth;
			pos.m_y = floatHeight - pos.m_y * 0.5f * floatHeight;
		}

		// Get the bounding box of the triangle
		const int minX = static_cast<int>(LibMath::min(points[0].m_x,
			LibMath::min(points[1].m_x, points[2].m_x)));

		const int minY = static_cast<int>(LibMath::min(points[0].m_y,
			LibMath::min(points[1].m_y, points[2].m_y)));

		const int maxX = static_cast<int>(LibMath::max(points[0].m_x,
			LibMath::max(points[1].m_x, points[2].m_x)));

		const int maxY = static_cast<int>(LibMath::max(points[0].m_y,
			LibMath::max(points[1].m_y, points[2].m_y)));

		// Spanning vectors of edge (v1,v2) and (v1,v3)
		const LibMath::Vector2 vs1(points[1].m_x - points[0].m_x,
			points[1].m_y - points[0].m_y);

		const LibMath::Vector2 vs2(points[2].m_x - points[0].m_x,
			points[2].m_y - points[0].m_y);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				LibMath::Vector2 q(static_cast<float>(x) - points[0].m_x,
					static_cast<float>(y) - points[0].m_y);

				const float s = q.cross(vs2) / vs1.cross(vs2);
				const float t = vs1.cross(q) / vs1.cross(vs2);

				if (s >= 0 && t >= 0 && s + t <= 1)
				{
					const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;
					float pixelZ = LibMath::lerp(p_vertices[0].m_position.m_z, p_vertices[1].m_position.m_z, s);
					pixelZ = LibMath::lerp(pixelZ, p_vertices[2].m_position.m_z, t);

					if (pixelZ < m_zBuffer[bufferIndex])
					{
						Color pixelColor = Color::lerp(p_vertices[0].m_color, p_vertices[1].m_color, s);
						pixelColor = Color::lerp(pixelColor, p_vertices[2].m_color, t);

						p_target.setPixelColor(x, y, pixelColor);
						m_zBuffer[bufferIndex] = pixelZ;
					}
				}
			}
		}
	}
}
