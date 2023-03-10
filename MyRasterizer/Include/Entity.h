#pragma once
#include "ITransformable.h"
#include "Matrix/Matrix4.h"
#include "Matrix/Matrix3.h"
#include "Vector/Vector3.h"
#include "Angle/Radian.h"


namespace My
{
	class Mesh;

	class Entity : public ITransformable
	{
		// Syntactic sugar for shorter lines
		using Mat4 = LibMath::Matrix4;
		using Vec3 = LibMath::Vector3;
		using Rad = LibMath::Radian;

	public:
		#pragma region Constructors
		explicit	Entity(const Mesh& p_mesh, const Mat4& p_transform = Mat4()); // TODO : Finish Entity comments
		Entity(const Mesh& p_mesh, float p_transparency, const Mat4& p_transform = Mat4());
		Entity(const Entity& p_other) = default;
		Entity(Entity&& p_other) = default;
		~Entity() = default;
		#pragma endregion

		#pragma region Getters
		/// <summary>
		/// Get the mesh the entity is based on
		/// </summary>
		/// <returns></returns>
		const Mesh* getMesh() const;

		/// <summary>
		/// Get the entity's transparency value
		/// </summary>
		/// <returns>Transparency in percent [0,1]</returns>
		float getTransparency() const;

		/// <summary>
		/// Checks whether the entity is fully opaque or not
		/// </summary>
		/// <returns>True if the entity is fully opaque. False otherwise</returns>
		bool isOpaque() const;
		#pragma endregion

		#pragma region Operators
		/// <summary>
		/// Copy operator
		/// </summary>
		/// <param name="p_other"></param>
		/// <returns></returns>
		Entity& operator=(const Entity& p_other) = default;

		/// <summary>
		/// Move operator
		/// </summary>
		/// <param name="p_other"></param>
		/// <returns></returns>
		Entity& operator=(Entity&& p_other) = default;
#pragma endregion

	private:
		const Mesh* m_mesh = nullptr;
		float		m_transparency;
	};
}

