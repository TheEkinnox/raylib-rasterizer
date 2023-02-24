#pragma once
#include "Matrix/Matrix4.h"

namespace My
{
	class Mesh;

	class Entity
	{
	public:
		explicit			Entity(const Mesh& p_mesh);
							Entity(const Entity& p_other) = default;
							Entity(Entity&& p_other) = default;
							~Entity() = default;

		Entity&				operator=(const Entity& p_other) = default;
		Entity&				operator=(Entity&& p_other) = default;

	private:
		const Mesh*			m_mesh;
		LibMath::Matrix4	m_transform;
	};
}

