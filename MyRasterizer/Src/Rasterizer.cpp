#include "Rasterizer.h"

#include "Arithmetic.h"
#include "Color.h"
#include "Mesh.h"
#include "Texture.h"
#include "Scene.h"
#include "Light.h"
#include "Vector/Vector2.h"
#include "Vector/Vector4.h"

namespace My
{
	Rasterizer::Rasterizer(const uint8_t p_sampleCount)
		: m_sampleCount(p_sampleCount), m_target(nullptr)
	{
		if (p_sampleCount == 0)
			throw std::invalid_argument(
				"Sample count must be greater than or equal to 1. Received: "
				+ std::to_string(p_sampleCount));
	}

	void Rasterizer::renderScene(const Scene& p_scene, Texture& p_target,
	                             const Mat4& p_projectionMatrix)
	{
		Texture target = p_target;

		if (!LibMath::floatEquals(m_sampleCount, 1.f) || m_target == &p_target)
		{
			const uint32_t msaaWidth = (p_target.getWidth() + 1) * m_sampleCount;
			const uint32_t msaaHeight = (p_target.getHeight() + 1) * m_sampleCount;

			target = Texture(msaaWidth, msaaHeight);
		}

		m_target = &target;

		// Set every pixel to black
		for (uint32_t x = 0; x < m_target->getWidth(); x++)
			for (uint32_t y = 0; y < m_target->getHeight(); y++)
				m_target->setPixelColor(x, y, Color::black);

		const size_t textureSize = static_cast<size_t>(m_target->getWidth())
			* m_target->getHeight();

		m_zBuffer.clear();
		m_zBuffer.resize(textureSize, INFINITY);

		const auto entities = p_scene.getEntities();
		std::vector<const Entity*> transparentEntities;

		for (const auto& entity : entities)
		{
			// Draw the opaque entities and defer the transparent ones' rendering
			if (entity.isOpaque())
				drawEntity(entity, *m_target, p_projectionMatrix, p_scene.getLights(), LibMath::Vector3::zero());
			else
				transparentEntities.push_back(&entity);

			//drawNormals(entity, *m_target, LibMath::Vector3::zero());
		}

		for (const auto& entityPtr : transparentEntities)
			drawEntity(*entityPtr, *m_target, p_projectionMatrix, p_scene.getLights(), LibMath::Vector3::zero());

		
		if (m_target == &p_target)
		{
			m_target = nullptr;
			return;
		}

		const LibMath::Vector2 deltaSize(	static_cast<float>(p_target.getWidth()),
									static_cast<float>(p_target.getHeight()));

		const float floatSampleCount = m_sampleCount;

		for (uint32_t x = 0; x < p_target.getWidth(); x++) //draw form msaa to p_texture
		{
			for (uint32_t y = 0; y < p_target.getHeight(); y++)
			{
				const float msaaX = floatSampleCount * static_cast<float>(x) + floatSampleCount * .5f;
				const float msaaY = floatSampleCount * static_cast<float>(y) + floatSampleCount * .5f;

				p_target.setPixelColor(x, y,
					m_target->getPixelColorBlerp(msaaX, msaaY, deltaSize));
			}
		}

		m_target = nullptr;
	}

	void Rasterizer::drawEntity(const Entity& p_entity, Texture& p_target,
		const Mat4& p_projectionMatrix, const std::vector<Light>& p_lights,
		const LibMath::Vector3& p_viewPos)
	{
		if (p_entity.getMesh() != nullptr)
		{
			auto vertices = p_entity.getMesh()->getVertices();
			auto normals = p_entity.getMesh()->getNormals();
			const auto indices = p_entity.getMesh()->getIndices();
			const Mat4& mvpMatrix = p_projectionMatrix;

			for (auto& v : vertices)
			{
				// Update vertex positions
				const Vec3 pos = v.m_position;
				Vec4 vec4 = { pos.m_x, pos.m_y, pos.m_z, 1.f };
				vec4 = p_entity.getTransform() * vec4;
				v.m_position = { vec4.m_x, vec4.m_y, vec4.m_z };

				// Update vertex normals
				auto& nor = v.m_normal;
				vec4 = LibMath::Vector4{ nor.m_x, nor.m_y, nor.m_z, 0.f };
				vec4 = p_entity.getRotation() * vec4;
				nor = { vec4.m_x, vec4.m_y, vec4.m_z };

				// Update vertex alpha
				const float floatAlpha = v.m_color.m_a;
				v.m_color.m_a = static_cast<uint8_t>(floatAlpha * p_entity.getTransparency());
			}

			for (size_t i = 0; i + 2 < indices.size(); i += 3)
			{
				auto& normal = normals[i / 3];
				LibMath::Vector4 normal4 = LibMath::Vector4(normal.m_x, normal.m_y, normal.m_z, 0.f);
				normal4 = p_entity.getRotation() * normal4;
				normal = { normal4.m_x, normal4.m_y, normal4.m_z };

				const Vertex triangle[3]
				{
					vertices[indices[i]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				};

				Vec3 centerPt = (triangle[0].m_position + triangle[1].m_position + triangle[2].m_position) / 3;

				if (shouldDrawFace(centerPt, normal, p_viewPos)
					&& checkFacingDirection(centerPt, p_viewPos, Vec3::front()))
					m_drawTriangle(triangle, p_target, p_lights, p_viewPos, mvpMatrix,
						p_entity.getMesh()->getTexture(), m_zBuffer);
			}
		}
	}

	//void Rasterizer::drawNormals(const Entity& p_entity, Texture& p_target,
	//	const LibMath::Vector3& p_viewPos)
	//{
	//	if (p_entity.getMesh() == nullptr)
	//		return;

	//	const auto vertices = p_entity.getMesh()->getVertices();
	//	const auto indices = p_entity.getMesh()->getIndices();

	//	for (auto v : vertices)
	//	{
	//		{
	//			auto& pos = v.m_position; //update pos
	//			auto vec4 = LibMath::Vector4{ pos.m_x, pos.m_y, pos.m_z, 1.f };
	//			vec4 = p_entity.getTransform() * vec4;
	//			pos = { vec4.m_x, vec4.m_y, vec4.m_z };
	//		}

	//		{
	//			auto& nor = v.m_normal; //update normal
	//			auto vec4 = LibMath::Vector4{ nor.m_x, nor.m_y, nor.m_z, 1.f };
	//			vec4 = p_entity.getRotation() * vec4;
	//			nor = { vec4.m_x, vec4.m_y, vec4.m_z };
	//		}

	//		const Color color = Color::white;

	//		const Vertex triangle1[3]
	//		{
	//			Vertex{v.m_position + LibMath::Vector3(0.02f), LibMath::Vector3::zero(), color},
	//			Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),color},
	//			Vertex{v.m_position,LibMath::Vector3::zero(),color}
	//		};

	//		drawTriangleFill(triangle1, p_target, {}, p_viewPos, m_zBuffer);

	//		const Vertex triangle2[3]
	//		{
	//			Vertex{v.m_position + LibMath::Vector3(0.02f),LibMath::Vector3::zero(), color},
	//			Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(),color},
	//			Vertex{v.m_position + v.m_normal + LibMath::Vector3(0.02f),LibMath::Vector3::zero(),color }
	//		};

	//		drawTriangleFill(triangle2, p_target, {}, p_viewPos, m_zBuffer);
	//	}
	//}

	void Rasterizer::drawTriangleFill(const Vertex p_vertices[3], Texture& p_target,
		const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
		const Mat4& p_mvpMatrix, const Texture* p_texture, std::vector<float>& p_zBuffer)
	{
		// Create an array of vector4 for the positions
		Vec3 points[3]
		{
			{ p_vertices[0].m_position.m_x, p_vertices[0].m_position.m_y, p_vertices[0].m_position.m_z },
			{ p_vertices[1].m_position.m_x, p_vertices[1].m_position.m_y, p_vertices[1].m_position.m_z },
			{ p_vertices[2].m_position.m_x, p_vertices[2].m_position.m_y, p_vertices[2].m_position.m_z }
		};

		for (auto& point : points)
		{
			// Convert vertex coordinates from world to screen
			// THIS MUST BE DONE AFTER LIGHTING - LIGHTS USE WORLD POSITIONS
			point = worldToPixel(point, p_target, p_mvpMatrix);
		}

		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		// Get the bounding box of the triangle
		const int minX = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_x,
			LibMath::min(points[1].m_x, points[2].m_x))));

		const int minY = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_y,
			LibMath::min(points[1].m_y, points[2].m_y))));

		const int maxX = static_cast<int>(LibMath::min(floatWidth - 1, LibMath::max(points[0].m_x,
			LibMath::max(points[1].m_x, points[2].m_x))));

		const int maxY = static_cast<int>(LibMath::min(floatHeight - 1, LibMath::max(points[0].m_y,
			LibMath::max(points[1].m_y, points[2].m_y))));

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

				if (!fillCanDrawPixel(LibMath::Vector3(s, t, w)))
					continue;

				const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;

				const LibMath::Vector3 pos = points[0] * s
					+ points[1] * t
					+ points[2] * w;

				if (pos.m_z >= p_zBuffer[bufferIndex])
					continue;

				Color pixelColor = p_vertices[0].m_color * s
					+ p_vertices[1].m_color * t
					+ p_vertices[2].m_color * w;

				// Round up alpha to account for precision loss
				pixelColor.m_a = pixelColor.m_a >= UINT8_MAX - 2 ? UINT8_MAX : pixelColor.m_a;

				if (pixelColor.m_a != UINT8_MAX)
					pixelColor.blend(p_target.getPixelColor(x, y));
				else
					p_zBuffer[bufferIndex] = pos.m_z;

				if (p_texture != nullptr)
				{
					const float u = p_vertices[0].m_u * s + p_vertices[1].m_u * t + p_vertices[2].m_u * w;
					const float v = p_vertices[0].m_v * s + p_vertices[1].m_v * t + p_vertices[2].m_v * w;

					const float textureX = (u - LibMath::floor(u))
						* static_cast<float>(p_texture->getWidth());

					const float textureY = (v - LibMath::floor(v))
						* static_cast<float>(p_texture->getHeight());

					pixelColor *= p_texture->getPixelColor(static_cast<uint32_t>(LibMath::round(textureX)),
						static_cast<uint32_t>(LibMath::round(textureY)));
				}

				if (!p_lights.empty())
				{
					const LibMath::Vector3 vertPos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;

					LibMath::Vector3 normal = p_vertices[0].m_normal * s
						+ p_vertices[1].m_normal * t
						+ p_vertices[2].m_normal * w;

					normal.normalize();

					Color litColor = Color::black;

					for (const auto& light : p_lights)
						litColor += light.calculateLightingBlinnPhong(vertPos, pixelColor, normal, p_viewPos);

					pixelColor = litColor;
				}

				p_target.setPixelColor(x, y, pixelColor);
			}
		}
	}

	void Rasterizer::drawTriangleWireFrame(const Vertex p_vertices[3], Texture& p_target,
		const std::vector<Light>& p_lights, const LibMath::Vector3& p_viewPos,
		const Mat4& p_mvpMatrix, const Texture* p_texture, std::vector<float>& p_zBuffer)
	{
		// Create an array of vector4 for the positions
		Vec3 points[3]
		{
			{ p_vertices[0].m_position.m_x, p_vertices[0].m_position.m_y, p_vertices[0].m_position.m_z },
			{ p_vertices[1].m_position.m_x, p_vertices[1].m_position.m_y, p_vertices[1].m_position.m_z },
			{ p_vertices[2].m_position.m_x, p_vertices[2].m_position.m_y, p_vertices[2].m_position.m_z }
		};

		for (auto& point : points)
		{
			// Convert vertex coordinates from world to screen
			// THIS MUST BE DONE AFTER LIGHTING - LIGHTS USE WORLD POSITIONS
			point = worldToPixel(point, p_target, p_mvpMatrix);
		}

		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		// Get the bounding box of the triangle
		const int minX = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_x,
			LibMath::min(points[1].m_x, points[2].m_x))));

		const int minY = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_y,
			LibMath::min(points[1].m_y, points[2].m_y))));

		const int maxX = static_cast<int>(LibMath::min(floatWidth - 1, LibMath::max(points[0].m_x,
			LibMath::max(points[1].m_x, points[2].m_x))));

		const int maxY = static_cast<int>(LibMath::min(floatHeight - 1, LibMath::max(points[0].m_y,
			LibMath::max(points[1].m_y, points[2].m_y))));

		// Spanning vectors of edge (v1,v2) and (v1,v3)
		const LibMath::Vector2 vs1(points[1].m_x - points[0].m_x,
			points[1].m_y - points[0].m_y);

		const LibMath::Vector2 vs2(points[2].m_x - points[0].m_x,
			points[2].m_y - points[0].m_y);

		// Delta Triangle size * delta UV's
		const float deltaU =	LibMath::max(LibMath::max(p_vertices[0].m_u, p_vertices[1].m_u), p_vertices[2].m_u) -
						LibMath::min(LibMath::min(p_vertices[0].m_u, p_vertices[1].m_u), p_vertices[2].m_u);
		const float deltaV =	LibMath::max(LibMath::max(p_vertices[0].m_v, p_vertices[1].m_v), p_vertices[2].m_v) -
						LibMath::min(LibMath::min(p_vertices[0].m_v, p_vertices[1].m_v), p_vertices[2].m_v);

		const LibMath::Vector2 deltaTriangleBounds = LibMath::Vector2(	static_cast<float>(maxX - minX) * deltaU,
																		static_cast<float>(maxY - minY) * deltaV);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				LibMath::Vector2 q(static_cast<float>(x) - points[0].m_x,
					static_cast<float>(y) - points[0].m_y);

				const float t = q.cross(vs2) / vs1.cross(vs2);
				const float w = vs1.cross(q) / vs1.cross(vs2);
				const float s = 1 - t - w;

				const LibMath::Vector3 pos = points[0] * s
					+ points[1] * t
					+ points[2] * w;

				const LibMath::Vector2 pixelPoints[3]
				{
					{ points[0].m_x, points[0].m_y },
					{ points[1].m_x, points[1].m_y },
					{ points[2].m_x, points[2].m_y }
				};

				if (!wireFrameCanDrawPixel({ pos.m_x, pos.m_y }, pixelPoints, LibMath::Vector3(s, t, w)))
					continue;

				const size_t bufferIndex = static_cast<size_t>(y) * p_target.getWidth() + x;

				if (pos.m_z >= p_zBuffer[bufferIndex])
					continue;

				Color pixelColor = p_vertices[0].m_color * s
					+ p_vertices[1].m_color * t
					+ p_vertices[2].m_color * w;

				// Round up alpha to account for precision loss
				pixelColor.m_a = pixelColor.m_a >= UINT8_MAX - 2 ? UINT8_MAX : pixelColor.m_a;

				if (pixelColor.m_a != UINT8_MAX)
					pixelColor.blend(p_target.getPixelColor(x, y));
				else
					p_zBuffer[bufferIndex] = pos.m_z;

				if (p_texture != nullptr)
				{
					const float u = p_vertices[0].m_u * s + p_vertices[1].m_u * t + p_vertices[2].m_u * w;
					const float v = p_vertices[0].m_v * s + p_vertices[1].m_v * t + p_vertices[2].m_v * w;

					const float textureX = (u - LibMath::floor(u))
						* static_cast<float>(p_texture->getWidth());

					const float textureY = (v - LibMath::floor(v))
						* static_cast<float>(p_texture->getHeight());

					/*pixelColor *= p_texture->getPixelColor(static_cast<uint32_t>(LibMath::round(textureX)),
						static_cast<uint32_t>(LibMath::round(textureY)));*/
					pixelColor *= p_texture->getPixelColorBlerp(textureX, textureY, deltaTriangleBounds);
				}

				if (!p_lights.empty())
				{
					const LibMath::Vector3 vertPos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;

					LibMath::Vector3 normal = p_vertices[0].m_normal * s
						+ p_vertices[1].m_normal * t
						+ p_vertices[2].m_normal * w;

					normal.normalize();

					Color litColor = Color::black;

					for (const auto& light : p_lights)
						litColor += light.calculateLightingBlinnPhong(vertPos, pixelColor, normal, p_viewPos);

					pixelColor = litColor;
				}

				p_target.setPixelColor(x, y, pixelColor);
			}
		}
	}

	void Rasterizer::toggleWireFrameMode()
	{
		{
			switch (m_drawMode)
			{
			case e_drawMode::E_FILL:
				m_drawMode = e_drawMode::E_WIRE_FRAME;
				m_drawTriangle = &Rasterizer::drawTriangleWireFrame;
				break;
			case e_drawMode::E_WIRE_FRAME:
				m_drawMode = e_drawMode::E_FILL;
				m_drawTriangle = &Rasterizer::drawTriangleFill;
				break;
			default:
				break;
			}
		}
	}

	LibMath::Vector3 Rasterizer::worldToPixel(const Vec3& p_pos, const Texture& p_target,
		const Mat4& p_mvpMatrix)
	{
		// Convert position to Vector4
		Vec4 projectedVec = { p_pos.m_x, p_pos.m_y, p_pos.m_z, 1.f };

		// Apply model view projection matrix
		projectedVec = p_mvpMatrix * projectedVec;

		// Convert projected coordinates to ndc
		projectedVec /= projectedVec.m_w;

		// Convert ndc coordinates to pixel
		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		projectedVec.m_x = (projectedVec.m_x + 1.f) / (2.f / floatWidth);
		projectedVec.m_y = (1.f - projectedVec.m_y) / (2.f / floatHeight);

		return { projectedVec.m_x, projectedVec.m_y, projectedVec.m_z };
	}

	LibMath::Vector2 Rasterizer::pointOnTriangleEdge(LibMath::Vector2 p_point, const LibMath::Vector2& p_triangleP1,
														const LibMath::Vector2& p_triangleP2)
	{
		p_point -= p_triangleP1;
		p_point.projectOnto(p_triangleP2 - p_triangleP1);

		return p_point + p_triangleP1;
	}

	bool Rasterizer::fillCanDrawPixel(const LibMath::Vector3& p_stw)
	{
		return p_stw.m_x >= 0 && p_stw.m_y >= 0 && p_stw.m_z >= 0 && p_stw.m_y + p_stw.m_z <= 1;
	}

	bool Rasterizer::wireFrameCanDrawPixel(	const LibMath::Vector2& p_pixelPos, const LibMath::Vector2 p_trianglePoints[3],
											const LibMath::Vector3& p_stw)
	{
		return fillCanDrawPixel(p_stw) && //is in triangle 
				(pointOnTriangleEdge(p_pixelPos, p_trianglePoints[0], p_trianglePoints[1]). //is on an edge
				inBounds(p_pixelPos - 1, p_pixelPos + 1) ||
				pointOnTriangleEdge(p_pixelPos, p_trianglePoints[1], p_trianglePoints[2]).
				inBounds(p_pixelPos - 1, p_pixelPos + 1) ||
				pointOnTriangleEdge(p_pixelPos, p_trianglePoints[2], p_trianglePoints[0]).
				inBounds(p_pixelPos - 1, p_pixelPos + 1));
	}

	bool Rasterizer::shouldDrawFace(const Vec3& p_trianglePos, const Vec3& p_triangleNormal,
		const Vec3& p_viewPos)
	{
		/*
		* https://en.wikipedia.org/wiki/Back-face_culling
		*/
		const Vec3 deltaPos = p_trianglePos - p_viewPos; //bcs constant

		return deltaPos.dot(p_triangleNormal) < 0;
	}

	bool Rasterizer::checkFacingDirection(const Vec3& p_trianglePos, const Vec3& p_observerPos,
		const Vec3& p_observerDir)
	{
		const Vec3 deltaPos = p_trianglePos - p_observerPos; //bcs constant

		return	deltaPos.dot(p_observerDir) <= 0;
	}
}
