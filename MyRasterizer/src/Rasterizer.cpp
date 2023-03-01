#include "Rasterizer.h"

#include "Arithmetic.h"
#include "Color.h"
#include "Mesh.h"
#include "Texture.h"
#include "Scene.h"
#include "Vector/Vector2.h"
#include "Vector/Vector4.h"
#include <Interpolation.h>

namespace My
{
	void Rasterizer::renderScene(const Scene& p_scene, Texture& p_target,
		const Mat4& p_projectionMatrix)
	{
		// Set every pixel to black
		for (uint32_t x = 0; x < p_target.getWidth(); x++)
			for (uint32_t y = 0; y < p_target.getHeight(); y++)
				p_target.setPixelColor(x, y, Color::black);

		for (const auto& entity : p_scene.getEntities())
			drawEntity(entity, p_target, p_projectionMatrix);
	}

	void Rasterizer::drawEntity(const Entity& p_entity, Texture& p_target,
		const Mat4& p_projectionMatrix)
	{
		if (p_entity.getMesh() != nullptr)
		{
			const auto vertices = p_entity.getMesh()->getVertices();
			const auto indices = p_entity.getMesh()->getIndices();

			for (size_t i = 0; i + 2 < indices.size(); i += 3)
			{
				const Vertex triangle[3]
				{
					vertices[indices[i]],
					vertices[indices[i + 1]],
					vertices[indices[i + 2]]
				};

				const Mat4 mvpMat = p_projectionMatrix * p_entity.getTransform();

				drawTriangle(triangle, p_target, mvpMat);
			}
		}
	}

	void Rasterizer::drawTriangle(const Vertex p_vertices[3], Texture& p_target,
		const Mat4& p_mvpMatrix)
	{
		// Create an array of vector4 for the positions
		LibMath::Vector4 points[3]
		{
			{p_vertices[0].m_position.m_x, p_vertices[0].m_position.m_y, p_vertices[0].m_position.m_z, 1.f},
			{p_vertices[1].m_position.m_x, p_vertices[1].m_position.m_y, p_vertices[1].m_position.m_z, 1.f},
			{p_vertices[2].m_position.m_x, p_vertices[2].m_position.m_y, p_vertices[2].m_position.m_z, 1.f}
		};

		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		// Project each point on the screen
		for (LibMath::Vector4& pos : points)
			pos = worldToPixel(pos, p_target, p_mvpMatrix);

		/* get the bounding box of the triangle */
		const int minX = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_x,
			LibMath::min(points[1].m_x, points[2].m_x))));

		const int minY = static_cast<int>(LibMath::max(0.f, LibMath::min(points[0].m_y,
			LibMath::min(points[1].m_y, points[2].m_y))));

		const int maxX = static_cast<int>(LibMath::min(floatWidth - 1, LibMath::max(points[0].m_x,
			LibMath::max(points[1].m_x, points[2].m_x))));

		const int maxY = static_cast<int>(LibMath::min(floatHeight - 1, LibMath::max(points[0].m_y,
			LibMath::max(points[1].m_y, points[2].m_y))));

		/* spanning vectors of edge (v1,v2) and (v1,v3) */
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

				float pixelZ = LibMath::lerp(points[0].m_z, points[1].m_z, s);
				pixelZ = LibMath::lerp(pixelZ, points[2].m_z, t);

				if (LibMath::abs(pixelZ) > 1)
					continue;

				Color pixelColor = Color::lerp(p_vertices[0].m_color, p_vertices[1].m_color, s);
				pixelColor = Color::lerp(pixelColor, p_vertices[2].m_color, t);

				if (s >= 0 && t >= 0 && s + t <= 1)
					p_target.setPixelColor(x, y, pixelColor);
			}
		}
	}

	LibMath::Vector4 Rasterizer::worldToPixel(const Vec4& p_pos, const Texture& p_target,
		const Mat4& p_mvpMatrix)
	{
		// Apply model view projection matrix
		Vec4 projectedVec = p_mvpMatrix * p_pos;

		// Convert projected coordinates to ndc
		projectedVec /= projectedVec.m_w;

		// Convert ndc coordinates to pixel
		const float floatWidth = static_cast<float>(p_target.getWidth());
		const float floatHeight = static_cast<float>(p_target.getHeight());

		projectedVec.m_x = (projectedVec.m_x + 1.f) / (2.f / floatWidth);
		projectedVec.m_y = (1.f - projectedVec.m_y) / (2.f / floatHeight);

		return projectedVec;
	}
}
