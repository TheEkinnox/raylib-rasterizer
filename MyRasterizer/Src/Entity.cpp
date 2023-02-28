#include "Entity.h"

#include <utility>

#include "Arithmetic.h"
#include "Trigonometry.h"

My::Entity::Entity(const Mesh& p_mesh, Mat4 p_transform) :
	m_mesh(&p_mesh), m_transform(std::move(p_transform))
{
	this->m_mesh = &p_mesh;
}

My::Entity& My::Entity::Translate(float p_x, float p_y, float p_z)
{
	this->m_transform *= Mat4::translation(p_x, p_y, p_z);
	return *this;
}

My::Entity& My::Entity::Scale(float p_x, float p_y, float p_z)
{
	this->m_transform *= Mat4::scaling(p_x, p_y, p_z);
	return *this;
}

My::Entity& My::Entity::RotateEulerAngles(Rad p_x, Rad p_y, Rad p_z)
{
	this->m_transform *= Mat4::rotation(p_x, Vec3::right());	//rotate x axis
	this->m_transform *= Mat4::rotation(p_y, Vec3::up());		//rotate y axis
	this->m_transform *= Mat4::rotation(p_z, Vec3::back());		//rotate z axis
	return *this;
}

My::Entity& My::Entity::SetPosition(float p_x, float p_y, float p_z)
{
	LibMath::Vector3 pos = this->GetPosition();
	//this->Translate(-pos.m_x, -pos.m_y, -pos.m_z); // back to (0, 0, 0)
	//this->Translate(p_x, p_y, p_z); // to SetPos
	this->Translate(	p_x - pos.m_x,
						p_y - pos.m_y, 
						p_z - pos.m_z);
	return *this;
}

My::Entity& My::Entity::SetScale(float p_x, float p_y, float p_z)
{
	LibMath::Vector3 scale = this->GetScale();
	//this->Scale(1 / scale.m_x, 1 / scale.m_y, 1 / scale.m_z); // back to (1, 1, 1)
	//this->Scale(p_x, p_y, p_z); // to Setscale
	this->Scale(	p_x / scale.m_x, 
					p_y / scale.m_y, 
					p_z / scale.m_z);

	return *this;
}

My::Entity& My::Entity::SetRotationEulerAngles(Rad p_x, Rad p_y, Rad p_z)
{
	LibMath::Vector3 rotateAngles = this->GetRotationEulerAngles();
	//this->RotateEulerAngles(	static_cast<Rad>(-rotateAngles.m_x), 
	//							static_cast<Rad>(-rotateAngles.m_y), 
	//							static_cast<Rad>(-rotateAngles.m_z)); // back to (0, 0, 0)
	//this->RotateEulerAngles(p_x, p_y, p_z); // to Setscale
	this->RotateEulerAngles(	p_x - static_cast<Rad>(rotateAngles.m_x),
								p_y - static_cast<Rad>(rotateAngles.m_y),
								p_z - static_cast<Rad>(rotateAngles.m_z)); // back to (0, 0, 0)

	return *this;
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

	return Vec3(this->GetScaleX(), this->GetScaleY(), this->GetScaleZ());
}

float My::Entity::GetScaleX() const
{
	//see GetScale()
	//
	return LibMath::squareRoot(	m_transform[0] * m_transform[0] +
								m_transform[4] * m_transform[4] +
								m_transform[8] * m_transform[8]);
}

float My::Entity::GetScaleY() const
{
	//see GetScale()
	//
	return LibMath::squareRoot(	m_transform[1] * m_transform[1] +
								m_transform[5] * m_transform[5] +
								m_transform[9] * m_transform[9]);
}

float My::Entity::GetScaleZ() const
{
	//see GetScale()
	//
	return LibMath::squareRoot(	m_transform[2] * m_transform[2] +
								m_transform[6] * m_transform[6] +
								m_transform[10] * m_transform[10]);
}

My::Entity::Vec3 My::Entity::GetRotationEulerAngles() const
{
	/**
	*                                          [ m00 m01 m02 ]
	* rotationMatrix = (1.0 / scalingFactor) * [ m10 m11 m12 ]
	*										   [ m20 m21 m22 ]
	*		|		  rx		  ux		  fx		   0 	 |
	* R =	|		  ry		  uy		  fy		   0 	 |
	*		|		  rz		  uz		  fz		   0 	 |
	*		|		   0		   0		   0		   1 	 |
	* http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
	*/
	Vec3 scale(this->GetScale());
	Mat4 rotation(this->m_transform);

	rotation.scaling(1.0f / scale.m_x, 1.0f / scale.m_y, 1.0f / scale.m_z); //un-scale mat
	//dont have to un-translate mat bcs trans doesnt affect rotation

	LibMath::Radian psi1, teta1, phi1, psi2, teta2, phi2;
	float cosTeta1, cosTeta2;

	if (LibMath::abs(rotation[8]) != 1) 
	{
		/**
		*		|		   0		   1		   2		   3 	 |
		* R =	|		   4		   5		   6		   7 	 |
		*		|		   8		   9		  10		   0 	 |
		*		|		   0		   0		   0		   1 	 |
		*/

		teta1 = -LibMath::asin(rotation[8]);
		teta2 = static_cast<Rad>(LibMath::g_pi) - teta1;

		cosTeta1 = LibMath::cos(teta1);
		cosTeta2 = LibMath::cos(teta2);

		psi1 = LibMath::atan(rotation[9] / cosTeta1, rotation[10] / cosTeta1);
		psi2 = LibMath::atan(rotation[9] / cosTeta2, rotation[10] / cosTeta2);

		phi1 = LibMath::atan(rotation[4] / cosTeta1, rotation[0] / cosTeta1);
		phi2 = LibMath::atan(rotation[4] / cosTeta2, rotation[0] / cosTeta2);
	}
	else //infinity solutions? / gimble lock
	{
		// TODO : Gimble lock exception Euler ANGLES, counld only give one?
	}

	//reverse phi bcs of global orientation
	phi1 *= -1;
	phi2 *= -1;

	// TODO : Get Rotation return? only 1 or 2 solutions? one is NAN sometimes
	//if (cosTeta1 != 0) //not NAN
	return Vec3(psi1.raw(), teta1.raw(), phi1.raw());
	//return Vec3(psi2.raw(), teta2.raw(), phi2.raw());
}

My::Entity::Vec3 My::Entity::GetRightward() const
{
	float xScale = this->GetScaleX();
	return Vec3( m_transform[0] / xScale, m_transform[4] / xScale, m_transform[8] / xScale );
}

My::Entity::Vec3 My::Entity::GetLeftward() const
{
	return (- 1 * this->GetRightward());
}

My::Entity::Vec3 My::Entity::GetUpward() const
{
	float yScale = this->GetScaleY();
	return Vec3(m_transform[1] / yScale, m_transform[5] / yScale,  m_transform[9] / yScale);
}

My::Entity::Vec3 My::Entity::GetDownward() const
{
	return (-1 * this->GetUpward());
}

My::Entity::Vec3 My::Entity::GetForward() const
{
	float zScale = this->GetScaleZ();
	return Vec3(m_transform[2] / zScale, m_transform[6] / zScale, m_transform[10] / zScale);
}

My::Entity::Vec3 My::Entity::GetBackward() const
{
	return (-1 * this->GetForward());
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
