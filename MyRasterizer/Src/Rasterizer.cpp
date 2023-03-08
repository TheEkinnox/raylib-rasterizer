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
#include <functional>

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

		auto entities = p_scene.getEntities();
		std::vector<const Entity*> transparentEntities;

		for (const auto& entity : entities)
		{
			if (entity.getTransparency() == 1)
				drawEntity(entity, p_target, p_scene.getLights()[0], m_drawTriangle);
			else //if transparent 
				transparentEntities.push_back(&entity);//add to vector to draw afterwards
			
			//drawNormals(entity, p_target, p_scene.getLights()[0]);
		}

		for (const auto& entityPtr : transparentEntities)
			drawEntity(*entityPtr, p_target, p_scene.getLights()[0], m_drawTriangle);

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

				drawTriangleFill(triangle, p_target, m_zBuffer);
			}
		}
	}

	void Rasterizer::drawEntity(const Entity& p_entity, Texture& p_target, const Light& p_light, drawFunc p_draw)
	{
		if (p_entity.getMesh() != nullptr)
		{
			auto vertices = p_entity.getMesh()->getVertices();
			auto indices = p_entity.getMesh()->getIndices();

			std::map<size_t, Color> vertexLightColor;

			//calculate light
			for (size_t i = 0; i < vertices.size(); i++)
			{
				Vertex& v = vertices[i];

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

				if (m_drawMode == e_drawMode::E_FILL)
				{
					v.m_color = p_light.CalculateLightingPhong(v, LibMath::Vector3::zero());
				}
				v.m_color.m_a = static_cast<uint8_t>(static_cast<float>(v.m_color.m_a) * p_entity.getTransparency());
			}

			for (size_t i = 0; i + 2 < indices.size(); i += 3)
			{
				Vertex triangle[3]
				{
					vertices[indices[i]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				};

				//triangle[0].m_color = vertexLightColor[indices[i]];
				//triangle[1].m_color = vertexLightColor[indices[i + 1]];
				//triangle[2].m_color = vertexLightColor[indices[i + 2]];

				//for (Vertex& vertex : triangle)
				//{
				//	auto& pos = vertex.m_position;
				//	auto vec4 = LibMath::Vector4{ pos.m_x, pos.m_y, pos.m_z, 1.f };

				//	vec4 = p_entity.getTransform() * vec4;
				//	pos = { vec4.m_x, vec4.m_y, vec4.m_z };
				//}

				p_draw(triangle, p_target, m_zBuffer);
			}
		}
	}

	void Rasterizer::drawNormals(const Entity& p_entity, Texture& p_target, const Light& p_light)
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

				Color color = p_light.CalculateLightingPhong(v, LibMath::Vector3::zero());

				Vertex triangle1[3]
				{
					Vertex{v.m_position + LibMath::Vector3(0.02f), LibMath::Vector3::zero(), color},
					Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),color},
					Vertex{v.m_position,LibMath::Vector3::zero(),color}
				};

				drawTriangleFill(triangle1, p_target, m_zBuffer);

				Vertex triangle2[3]
				{
					Vertex{v.m_position + LibMath::Vector3(0.02f),LibMath::Vector3::zero(), color},
					Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),color},
					Vertex{v.m_position + v.m_normal + LibMath::Vector3(0.02f),LibMath::Vector3::zero(),color }
				};

				drawTriangleFill(triangle2, p_target, m_zBuffer);
			}
		}
	}

	void Rasterizer::drawTriangleFill(const Vertex p_vertices[3], Texture& p_target, std::vector<float>& p_zBuffer)
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

				const float t = q.cross(vs2) / vs1.cross(vs2);
				const float w = vs1.cross(q) / vs1.cross(vs2);
				const float s = 1 - t - w;
				const LibMath::Vector2 pixelPos(static_cast<float>(x), static_cast<float>(y));
				
				if (Rasterizer::fillCanDrawPixel(LibMath::Vector3(s,t,w)))
				{
					const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;

					const LibMath::Vector3 pos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;
						
					Color pixelColor = p_vertices[0].m_color * s
						+ p_vertices[1].m_color * t
						+ p_vertices[2].m_color * w;

					pixelColor.m_a = pixelColor.m_a >= UINT8_MAX - 2 ? UINT8_MAX : pixelColor.m_a;

					if (pos.m_z < p_zBuffer[bufferIndex])
					{
						if (pixelColor.m_a != UINT8_MAX) //if transparent lerp from current color to new color
							pixelColor.blend(p_target.getPixelColor(x, y));
						else
							p_zBuffer[bufferIndex] = pos.m_z;

							
						p_target.setPixelColor(x, y, pixelColor);
					}
				}
			}
		}
	}
	void Rasterizer::drawTriangleWireFrame(const Vertex p_vertices[3], Texture& p_target, std::vector<float>& p_zBuffer)
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

		const LibMath::Vector2 trianglePoints[3] =
		{
			LibMath::Vector2(points[0].m_x, points[0].m_y),
			LibMath::Vector2(points[1].m_x, points[1].m_y),
			LibMath::Vector2(points[2].m_x, points[2].m_y),
		};

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				LibMath::Vector2 q(static_cast<float>(x) - points[0].m_x,
					static_cast<float>(y) - points[0].m_y);

				const float t = q.cross(vs2) / vs1.cross(vs2);
				const float w = vs1.cross(q) / vs1.cross(vs2);
				const float s = 1 - t - w;
				const LibMath::Vector2 pixelPos(static_cast<float>(x), static_cast<float>(y));

				if (Rasterizer::wireFrameCanDrawPixel(pixelPos, trianglePoints, LibMath::Vector3(s, t, w)))
				{
					const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;

					const LibMath::Vector3 pos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;

					if (pos.m_z < p_zBuffer[bufferIndex])
					{
						const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;

						const LibMath::Vector3 pos = p_vertices[0].m_position * s
							+ p_vertices[1].m_position * t
							+ p_vertices[2].m_position * w;

						Color pixelColor = p_vertices[0].m_color * s
							+ p_vertices[1].m_color * t
							+ p_vertices[2].m_color * w;

						pixelColor.m_a = UINT8_MAX;

						if (pos.m_z < p_zBuffer[bufferIndex])
						{
							p_zBuffer[bufferIndex] = pos.m_z;
							p_target.setPixelColor(x, y, pixelColor);
						}
					}
				}
			}
		}
	}

	void Rasterizer::toggleWireFrameMode()
	{
		{
			switch (m_drawMode)
			{
			case My::Rasterizer::e_drawMode::E_FILL:
				m_drawMode = My::Rasterizer::e_drawMode::E_WIRE_FRAME;
				m_drawTriangle = &Rasterizer::drawTriangleWireFrame;
				break;
			case My::Rasterizer::e_drawMode::E_WIRE_FRAME:
				m_drawMode = My::Rasterizer::e_drawMode::E_FILL;
				m_drawTriangle = &Rasterizer::drawTriangleFill;
				break;
			default:
				break;
			}
		}
	}

	LibMath::Vector2 Rasterizer::pointOnTriangleEdge(	LibMath::Vector2 p_point, const LibMath::Vector2& p_triangleP1, 
														const LibMath::Vector2& p_triangleP2)
	{
		p_point -= p_triangleP1;
		p_point.projectOnto(p_triangleP2 - p_triangleP1);

		return p_point + p_triangleP1;
	}

	bool Rasterizer::wireFrameCanDrawPixel(	const LibMath::Vector2& p_pixelPos, const LibMath::Vector2 p_trianglePoints[3],
											const LibMath::Vector3& p_stw)
	{
		return	Rasterizer::fillCanDrawPixel(p_stw) && //is in triangle 
				(pointOnTriangleEdge(p_pixelPos, p_trianglePoints[0], p_trianglePoints[1]). //is on an edge
				inBounds(p_pixelPos - 1, p_pixelPos + 1) ||
				pointOnTriangleEdge(p_pixelPos, p_trianglePoints[1], p_trianglePoints[2]).
				inBounds(p_pixelPos - 1, p_pixelPos + 1) ||
				pointOnTriangleEdge(p_pixelPos, p_trianglePoints[2], p_trianglePoints[0]).
				inBounds(p_pixelPos - 1, p_pixelPos + 1));
	}

	bool Rasterizer::fillCanDrawPixel(const LibMath::Vector3& p_stw)
	{
		return p_stw.m_x >= 0 && p_stw.m_y >= 0 && p_stw.m_z >= 0 && p_stw.m_y + p_stw.m_z <= 1;
	}
}
