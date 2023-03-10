#pragma once

#include "ITransformable.h"

namespace My
{
	class Camera : public ITransformable
	{
		using Mat4 = LibMath::Matrix4;

	public:
		Camera();

		/**
		 * \brief Creates a camera with the given transform and projection matrices
		 * \param p_transform The camera's transformation matrix
		 * \param p_projection The matrix to use to project points
		 * from world space to clip space
		 */
		Camera(const Mat4& p_transform, Mat4 p_projection);

		/**
		 * \brief Returns the view matrix from the camera's transformation matrix
		 * \return The inverse of the camera's transformation matrix
		 */
		Mat4 getViewMatrix() const;

		/**
		 * \brief Gives read access to the camera's projection matrix
		 * \return A copy of the camera's projection matrix
		 */
		Mat4 getProjectionMatrix() const;

		/**
		 * \brief Sets the camera's projection matrix
		 */
		void setProjectionMatrix(const Mat4& p_projection);

	private:
		Mat4	m_projectionMatrix;
	};
}
