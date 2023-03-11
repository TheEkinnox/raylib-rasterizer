#include "Rasterizer.h"

#include "Arithmetic.h"
#include "Camera.h"
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
		: m_sampleCount(p_sampleCount)
	{
		if (p_sampleCount == 0)
			throw std::invalid_argument(
				"Sample count must be greater than or equal to 1. Received: "
				+ std::to_string(p_sampleCount));
	}

	void Rasterizer::renderScene(const Scene& p_scene, const Camera& p_camera,
		Texture& p_target)
	{
		Texture target = p_target;

		if (!LibMath::floatEquals(m_sampleCount, 1.f))
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

		const auto& lights = p_scene.getLights();
		m_lights = &lights;

		m_camera = &p_camera;

		const auto entities = p_scene.getEntities();
		std::vector<const Entity*> transparentEntities;

		for (const auto& entity : entities)
		{
			// Draw the opaque entities and defer the transparent ones' rendering
			if (entity.isOpaque())
				drawEntity(entity);
			else
				transparentEntities.push_back(&entity);

			//drawNormals(entity);
		}

		for (const auto& entityPtr : transparentEntities)
			drawEntity(*entityPtr);

		
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
		m_camera = nullptr;
		m_lights = nullptr;
		m_zBuffer.clear();
	}

	void Rasterizer::drawEntity(const Entity& p_entity)
	{
		if (m_camera == nullptr || m_target == nullptr
			|| p_entity.getMesh() == nullptr)
			return;

		auto vertices = p_entity.getMesh()->getVertices();
		auto normals = p_entity.getMesh()->getNormals();
		const auto indices = p_entity.getMesh()->getIndices();

		std::vector<Vec3> pixelPoints;
		pixelPoints.reserve(vertices.size());

		// Model matrix not required since it's directly applied to vertices
		const Mat4 mvpMatrix = m_camera->getProjectionMatrix()
			* m_camera->getViewMatrix();

		const Vec3 viewPos = m_camera->getPosition();

		for (auto& v : vertices)
		{
			// Update vertex positions
			Vec3& pos = v.m_position;
			Vec4 vec4 = { pos.m_x, pos.m_y, pos.m_z, 1.f };
			vec4 = p_entity.getTransform() * vec4;
			pos = { vec4.m_x, vec4.m_y, vec4.m_z };

			pixelPoints.push_back(worldToPixel(pos, *m_target, mvpMatrix));

			// Update vertex normals
			auto& nor = v.m_normal;
			vec4 = { nor.m_x, nor.m_y, nor.m_z, 0.f };
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

			const Vec3 pixelTriangle[3]
			{
				pixelPoints[indices[i]],
				pixelPoints[indices[i + 1]],
				pixelPoints[indices[i + 2]]
			};

			Vec3 centerPt = (triangle[0].m_position + triangle[1].m_position + triangle[2].m_position) / 3;

			if (shouldDrawFace(centerPt, normal, viewPos)
				&& checkFacingDirection(centerPt, viewPos, m_camera->getForward()))
				m_drawTriangle(triangle, pixelTriangle, p_entity.getMesh()->getTexture(), *this);
		}
	}

	void Rasterizer::drawNormals(const Entity& p_entity)
	{
		if (p_entity.getMesh() == nullptr)
			return;

		auto vertices = p_entity.getMesh()->getVertices();
		const auto indices = p_entity.getMesh()->getIndices();

		// Model matrix not required since it's directly applied to vertices
		const Mat4 mvpMatrix = m_camera->getProjectionMatrix()
			* m_camera->getViewMatrix();

		for (auto& v : vertices)
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

			const Color color = Color::white;

			const Vertex triangle1[3]
			{
				Vertex{v.m_position + LibMath::Vector3(0.02f), LibMath::Vector3::zero(), color, 0, 0 },
				Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(), color, 0, 0 },
				Vertex{v.m_position,LibMath::Vector3::zero(), color, 0, 0 }
			};

			const Vec3 pixelTriangle1[3]
			{
				worldToPixel(triangle1[0].m_position, *m_target, mvpMatrix),
				worldToPixel(triangle1[1].m_position, *m_target, mvpMatrix),
				worldToPixel(triangle1[2].m_position, *m_target, mvpMatrix)
			};

			drawTriangleFill(triangle1, pixelTriangle1, nullptr, *this);

			const Vertex triangle2[3]
			{
				Vertex{v.m_position + LibMath::Vector3(0.02f),LibMath::Vector3::zero(), color, 0, 0 },
				Vertex{v.m_position + v.m_normal,LibMath::Vector3::zero(), color, 0, 0 },
				Vertex{v.m_position + v.m_normal + LibMath::Vector3(0.02f),LibMath::Vector3::zero(), color, 0, 0 }
			};

			const Vec3 pixelTriangle2[3]
			{
				worldToPixel(triangle2[0].m_position, *m_target, mvpMatrix),
				worldToPixel(triangle2[1].m_position, *m_target, mvpMatrix),
				worldToPixel(triangle2[2].m_position, *m_target, mvpMatrix)
			};

			drawTriangleFill(triangle2, pixelTriangle2, nullptr, *this);
		}
	}

	void Rasterizer::drawTriangleFill(const Vertex p_vertices[3], const Vec3 p_pixelTriangle[3], const Texture* p_texture,
		Rasterizer& p_self)
	{
		if (p_self.m_camera == nullptr || p_self.m_target == nullptr)
			return;

		const float floatWidth = static_cast<float>(p_self.m_target->getWidth());
		const float floatHeight = static_cast<float>(p_self.m_target->getHeight());

		// Get the bounding box of the triangle
		const int minX = static_cast<int>(LibMath::max(0.f, LibMath::min(p_pixelTriangle[0].m_x,
			LibMath::min(p_pixelTriangle[1].m_x, p_pixelTriangle[2].m_x))));

		const int minY = static_cast<int>(LibMath::max(0.f, LibMath::min(p_pixelTriangle[0].m_y,
			LibMath::min(p_pixelTriangle[1].m_y, p_pixelTriangle[2].m_y))));

		const int maxX = static_cast<int>(LibMath::min(floatWidth - 1, LibMath::max(p_pixelTriangle[0].m_x,
			LibMath::max(p_pixelTriangle[1].m_x, p_pixelTriangle[2].m_x))));

		const int maxY = static_cast<int>(LibMath::min(floatHeight - 1, LibMath::max(p_pixelTriangle[0].m_y,
			LibMath::max(p_pixelTriangle[1].m_y, p_pixelTriangle[2].m_y))));

		// Spanning vectors of edge (v1,v2) and (v1,v3)
		const LibMath::Vector2 vs1(p_pixelTriangle[1].m_x - p_pixelTriangle[0].m_x,
			p_pixelTriangle[1].m_y - p_pixelTriangle[0].m_y);

		const LibMath::Vector2 vs2(p_pixelTriangle[2].m_x - p_pixelTriangle[0].m_x,
			p_pixelTriangle[2].m_y - p_pixelTriangle[0].m_y);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				LibMath::Vector2 q(static_cast<float>(x) - p_pixelTriangle[0].m_x,
					static_cast<float>(y) - p_pixelTriangle[0].m_y);

				const float t = q.cross(vs2) / vs1.cross(vs2);
				const float w = vs1.cross(q) / vs1.cross(vs2);
				const float s = 1 - t - w;

				if (!fillCanDrawPixel(LibMath::Vector3(s, t, w)))
					continue;

				const size_t bufferIndex = static_cast<size_t>(y) * p_self.m_target->getWidth() + x;

				const float pixelZ = p_pixelTriangle[0].m_z * s
					+ p_pixelTriangle[1].m_z * t
					+ p_pixelTriangle[2].m_z * w;

				if (LibMath::abs(pixelZ) > 1 || pixelZ >= p_self.m_zBuffer[bufferIndex])
					continue;

				Color pixelColor = p_vertices[0].m_color * s
					+ p_vertices[1].m_color * t
					+ p_vertices[2].m_color * w;

				// Round up alpha to account for precision loss
				pixelColor.m_a = pixelColor.m_a >= UINT8_MAX - 2 ? UINT8_MAX : pixelColor.m_a;

				if (pixelColor.m_a != UINT8_MAX)
					pixelColor.blend(p_self.m_target->getPixelColor(x, y));
				else
					p_self.m_zBuffer[bufferIndex] = pixelZ;

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

				if (p_self.m_lights != nullptr && !p_self.m_lights->empty())
				{
					const LibMath::Vector3 vertPos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;

					LibMath::Vector3 normal = p_vertices[0].m_normal * s
						+ p_vertices[1].m_normal * t
						+ p_vertices[2].m_normal * w;

					normal.normalize();

					Color litColor = Color::black;

					for (const auto& light : *p_self.m_lights)
						litColor += light.calculateLightingBlinnPhong(vertPos, pixelColor, normal, p_self.m_camera->getPosition());

					pixelColor = litColor;
				}

				p_self.m_target->setPixelColor(x, y, pixelColor);
			}
		}
	}

	void Rasterizer::drawTriangleWireFrame(const Vertex p_vertices[3], const Vec3 p_pixelTriangle[3], const Texture* p_texture,
		Rasterizer& p_self)
	{
		if (p_self.m_camera == nullptr || p_self.m_target == nullptr)
			return;

		const float floatWidth = static_cast<float>(p_self.m_target->getWidth());
		const float floatHeight = static_cast<float>(p_self.m_target->getHeight());

		// Get the bounding box of the triangle
		const int minX = static_cast<int>(LibMath::max(0.f, LibMath::min(p_pixelTriangle[0].m_x,
			LibMath::min(p_pixelTriangle[1].m_x, p_pixelTriangle[2].m_x))));

		const int minY = static_cast<int>(LibMath::max(0.f, LibMath::min(p_pixelTriangle[0].m_y,
			LibMath::min(p_pixelTriangle[1].m_y, p_pixelTriangle[2].m_y))));

		const int maxX = static_cast<int>(LibMath::min(floatWidth - 1, LibMath::max(p_pixelTriangle[0].m_x,
			LibMath::max(p_pixelTriangle[1].m_x, p_pixelTriangle[2].m_x))));

		const int maxY = static_cast<int>(LibMath::min(floatHeight - 1, LibMath::max(p_pixelTriangle[0].m_y,
			LibMath::max(p_pixelTriangle[1].m_y, p_pixelTriangle[2].m_y))));

		// Spanning vectors of edge (v1,v2) and (v1,v3)
		const LibMath::Vector2 vs1(p_pixelTriangle[1].m_x - p_pixelTriangle[0].m_x,
			p_pixelTriangle[1].m_y - p_pixelTriangle[0].m_y);

		const LibMath::Vector2 vs2(p_pixelTriangle[2].m_x - p_pixelTriangle[0].m_x,
			p_pixelTriangle[2].m_y - p_pixelTriangle[0].m_y);

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
				LibMath::Vector2 q(static_cast<float>(x) - p_pixelTriangle[0].m_x,
					static_cast<float>(y) - p_pixelTriangle[0].m_y);

				const float t = q.cross(vs2) / vs1.cross(vs2);
				const float w = vs1.cross(q) / vs1.cross(vs2);
				const float s = 1 - t - w;

				const LibMath::Vector3 pos = p_pixelTriangle[0] * s
					+ p_pixelTriangle[1] * t
					+ p_pixelTriangle[2] * w;

				const LibMath::Vector2 pixelPoints[3]
				{
					{ p_pixelTriangle[0].m_x, p_pixelTriangle[0].m_y },
					{ p_pixelTriangle[1].m_x, p_pixelTriangle[1].m_y },
					{ p_pixelTriangle[2].m_x, p_pixelTriangle[2].m_y }
				};

				if (!wireFrameCanDrawPixel({ pos.m_x, pos.m_y }, pixelPoints, LibMath::Vector3(s, t, w)))
					continue;

				const size_t bufferIndex = static_cast<size_t>(y) * p_self.m_target->getWidth() + x;

				if (LibMath::abs(pos.m_z) > 1 || pos.m_z >= p_self.m_zBuffer[bufferIndex])
					continue;

				Color pixelColor = p_vertices[0].m_color * s
					+ p_vertices[1].m_color * t
					+ p_vertices[2].m_color * w;

				// Round up alpha to account for precision loss
				pixelColor.m_a = pixelColor.m_a >= UINT8_MAX - 2 ? UINT8_MAX : pixelColor.m_a;

				if (pixelColor.m_a != UINT8_MAX)
					pixelColor.blend(p_self.m_target->getPixelColor(x, y));
				else
					p_self.m_zBuffer[bufferIndex] = pos.m_z;

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

				if (p_self.m_lights != nullptr && !p_self.m_lights->empty())
				{
					const LibMath::Vector3 vertPos = p_vertices[0].m_position * s
						+ p_vertices[1].m_position * t
						+ p_vertices[2].m_position * w;

					LibMath::Vector3 normal = p_vertices[0].m_normal * s
						+ p_vertices[1].m_normal * t
						+ p_vertices[2].m_normal * w;

					normal.normalize();

					Color litColor = Color::black;

					for (const auto& light : *p_self.m_lights)
						litColor += light.calculateLightingBlinnPhong(vertPos, pixelColor, normal, p_self.m_camera->getPosition());

					pixelColor = litColor;
				}

				p_self.m_target->setPixelColor(x, y, pixelColor);
			}
		}
	}

	void Rasterizer::toggleWireFrameMode()
	{
		{
			switch (m_drawMode)
			{
			case EDrawMode::E_FILL:
				m_drawMode = EDrawMode::E_WIRE_FRAME;
				m_drawTriangle = &Rasterizer::drawTriangleWireFrame;
				break;
			case EDrawMode::E_WIRE_FRAME:
				m_drawMode = EDrawMode::E_FILL;
				m_drawTriangle = &Rasterizer::drawTriangleFill;
				break;
			default:
				throw Exceptions::InvalidDrawMode();
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
