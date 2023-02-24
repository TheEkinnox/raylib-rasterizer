#include "Entity.h"

#include <utility>

#include "Arithmetic.h"

My::Entity::Entity(const Mesh& p_mesh, Mat4 p_transform) :
	m_mesh(&p_mesh), m_transform(std::move(p_transform))
{
	this->m_mesh = &p_mesh;
}

My::Entity& My::Entity::Translate(float p_x, float p_y, float p_z)
{
	this->m_transform.translation(p_x, p_y, p_z);
	return *this;
}

My::Entity& My::Entity::Scale(float p_x, float p_y, float p_z)
{
	this->m_transform.scaling(p_x, p_y, p_z);
	return *this;
}

My::Entity& My::Entity::RotateEulerAngles(Rad p_x, Rad p_y, Rad p_z)
{
	this->m_transform.rotationEuler(p_x, p_y, p_z);
	return *this;
}

My::Entity& My::Entity::SetPosition(float p_x, float p_y, float p_z)
{
	// TODO: insert return statement here
}

My::Entity& My::Entity::SetScale(float p_x, float p_y, float p_z)
{
	// TODO: insert return statement here
}

My::Entity& My::Entity::SetRotationEulerAngles(Rad p_x, Rad p_y, Rad p_z)
{
	// TODO: insert return statement here
}

My::Entity::Vec3 My::Entity::GetPosition() const
{
	/**
	* 		|		   1		   0		   0		  tx 	 |
	* T =	|		   0		   1 		   0		  ty 	 |
	*		|		   0		   0		   1		  tz 	 |
	*		|		   0		   0		   0		   1 	 |
	*                     [ m03 ]
	* translationVector = [ m13 ]
    *	                  [ m23 ]
	*/

	return Vec3(	m_transform[3], 
					m_transform[7], 
					m_transform[11]);
}

My::Entity::Vec3 My::Entity::GetScale() const
{
	/**		
	* 		|		  sx		   0		   0		   0 	 |
	* S =	|		   0		  sy 		   0		   0 	 |
	*		|		   0		   0		  sz		   0 	 |
	*		|		   0		   0		   0		   1 	 |
	* 
	* scalingFactor = sqrt(m00 * m00 + m01 * m01 + m02 * m02);
	*/

	return Vec3(	LibMath::squareRoot(	m_transform[0] * m_transform[0] + 
											m_transform[4] * m_transform[4] + 
											m_transform[8] * m_transform[8]),

					LibMath::squareRoot(	m_transform[1] * m_transform[1] +
											m_transform[5] * m_transform[5] +
											m_transform[9] * m_transform[9]), 

					LibMath::squareRoot(	m_transform[2] * m_transform[2] +
											m_transform[6] * m_transform[6] +
											m_transform[10] * m_transform[10]));
}

My::Entity::Vec3 My::Entity::GetRotationEulerAngles() const
{
	/**
	*                                          [ m00 m01 m02 ]
	* rotationMatrix = (1.0 / scalingFactor) * [ m10 m11 m12 ]
	*										   [ m20 m21 m22 ]
	*/

	Vec3 scale(this->GetScale());
	Mat4 rotation = this->m_transform;

	rotation.scaling(1.0f / scale.m_x, 1.0f / scale.m_y, 1.0f / scale.m_z); //un-scale mat
	//dont have to un-translate mat bcs trans doesnt affect rotation
	
	

	return Vec3();
}

My::Entity::Vec3 My::Entity::GetRightward() const
{
	return Vec3();
}

My::Entity::Vec3 My::Entity::GetLeftward() const
{
	return Vec3();
}

My::Entity::Vec3 My::Entity::GetUpward() const
{
	return Vec3();
}

My::Entity::Vec3 My::Entity::GetDownward() const
{
	return Vec3();
}

My::Entity::Vec3 My::Entity::GetForward() const
{
	return Vec3();
}

My::Entity::Vec3 My::Entity::GetBackward() const
{
	return Vec3();
}

const My::Mesh* My::Entity::getMesh() const
{
	return m_mesh;
}

My::Entity::Mat4 My::Entity::getTransform() const
{
	return m_transform;
}

void My::Entity::setTransform(const Mat4& p_transform)
{
	m_transform = p_transform;
}
