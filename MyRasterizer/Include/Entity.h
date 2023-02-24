#pragma once
#include "Matrix/Matrix4.h"

namespace My
{
	class Mesh;

	class Entity
	{
		// Syntactic sugar for shorter lines
		using Mat4 = LibMath::Matrix4;

	public:
		explicit	Entity(const Mesh& p_mesh, Mat4 p_transform = Mat4());
					Entity(const Entity& p_other) = default;
					Entity(Entity&& p_other) = default;
					~Entity() = default;

		const Mesh*	getMesh() const;
		Mat4		getTransform() const;
		void		setTransform(const Mat4& p_transform);

		Entity&		operator=(const Entity& p_other) = default;
		Entity&		operator=(Entity&& p_other) = default;

	private:
		const Mesh*	m_mesh = nullptr;
		Mat4		m_transform;
	};
}

