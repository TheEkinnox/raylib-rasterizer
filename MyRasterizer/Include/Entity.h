#pragma once
#include <vector>
#include "Matrix/Matrix4.h"
#include "Vector/Vector3.h"
#include "Angle/Radian.h"


namespace My
{
	class Mesh;

	class Entity
	{
		// Syntactic sugar for shorter lines
		using Mat4 = LibMath::Matrix4;
		using Vec3 = LibMath::Vector3;
		using Rad = LibMath::Radian;

	public:
		explicit	Entity(const Mesh& p_mesh, Mat4 p_transform = Mat4());
					Entity(const Entity& p_other) = default;
					Entity(Entity&& p_other) = default;
					~Entity() = default;

		Entity& Translate(float p_x, float p_y, float p_z);
		//Entity& TranslateLocaly(float p_x, float p_y, float p_z); // ???
		Entity& Scale(float p_x, float p_y, float p_z);
		Entity& RotateEulerAngles(Rad p_x, Rad p_y, Rad p_z);

		Entity& SetPosition(float p_x, float p_y, float p_z);
		Entity& SetScale(float p_x, float p_y, float p_z);
		Entity& SetRotationEulerAngles(Rad p_x, Rad p_y, Rad p_z);

		Vec3 GetPosition()const;
		Vec3 GetScale()const;
		Vec3 GetRotationEulerAngles()const;

		float GetScaleX()const;
		float GetScaleY()const;
		float GetScaleZ()const;

		Vec3 GetRightward()const;
		Vec3 GetLeftward()const;
		Vec3 GetUpward()const;
		Vec3 GetDownward()const;
		Vec3 GetForward()const;
		Vec3 GetBackward()const;


		const Mesh*	getMesh() const;
		Mat4		getTransform() const;
		void		setTransform(const Mat4& p_transform);

		Entity&		operator=(const Entity& p_other) = default;
		Entity&		operator=(Entity&& p_other) = default;

	private:
		const Mesh* m_mesh = nullptr;
		Mat4		m_transform;
		
		/*
		* d = M * s
		* 
		* SourceColumnVector :			s = (x, y, z)
		* DestinationColumnVector :		d = (x, y, z)
		* 
		* 
		* 
		* M = T * R * S 
		* 
		*		|		sx * rx		sy * ux		sz * fx		  tx 	 |
		* M =	|		sx * ry		sy * uy		sz * fy		  ty 	 |
		*		|		sx * rz		sy * uz		sz * fz		  tz 	 |
		*		|		   0		   0		   0		   1 	 |
		* 
		* 		|		   1		   0		   0		  tx 	 |
		* T =	|		   0		   1 		   0		  ty 	 |
		*		|		   0		   0		   1		  tz 	 |
		*		|		   0		   0		   0		   1 	 |
		* 
		* 		|		  sx		   0		   0		   0 	 |
		* S =	|		   0		  sy 		   0		   0 	 |
		*		|		   0		   0		  sz		   0 	 |
		*		|		   0		   0		   0		   1 	 |
		* 
		*		|		  rx		  ux		  fx		   0 	 |
		* R =	|		  ry		  uy		  fy		   0 	 |
		*		|		  rz		  uz		  fz		   0 	 |
		*		|		   0		   0		   0		   1 	 |
		* 
		* rotates to new Righwards :	r = (x, y, z)
		* rotates to new Upwards :		u = (x, y, z)
		* rotates to new Forwards :		f = (x, y, z)
		* 
		* 
		* SOURCE : 
		* http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf
		* https://gamedev.stackexchange.com/questions/124667/how-does-a-4x4-matrix-represent-an-object-in-space-and-matrix-lore/124669#124669?newreg=23fefeecfcd746dfa5c747a8b4d5c072
		* https://www.euclideanspace.com/maths/geometry/affine/matrix4x4/index.htm
		* https://stackoverflow.com/questions/27655885/get-position-rotation-and-scale-from-matrix-in-opengl
		* 
		*/
	};
}

