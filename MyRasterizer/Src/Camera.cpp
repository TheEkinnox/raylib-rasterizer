#include "Camera.h"
#include "Angle/Degree.h"

using namespace LibMath::Literal;

namespace My
{
	Camera::Camera()
		: ITransformable(Mat4(1.f)),
		m_projectionMatrix(Mat4::perspectiveProjection(
			90_deg, 16.f / 9.f, .1f, 200.f))
	{
	}

	Camera::Camera(const Mat4& p_transform, Mat4 p_projection)
		: ITransformable(p_transform), m_projectionMatrix(std::move(p_projection))
	{
	}

	Camera::Mat4 Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}

	void Camera::setProjectionMatrix(const Mat4& p_projection)
	{
		m_projectionMatrix = p_projection;
	}

	Camera::Mat4 Camera::getViewMatrix() const
	{
		return getTransform().inverse();
	}
}
