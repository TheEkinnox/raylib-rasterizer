#pragma once
#include "Matrix/Matrix4.h"
#include "Matrix/Matrix3.h"
#include "Vector/Vector3.h"
#include "Angle/Radian.h"


namespace My
{
	namespace Exceptions
	{
		class DivideByZero : public std::exception
		{
		public:
			/**
			 * \brief Creates a divide by zero exception with a default message
			 */
			DivideByZero() :
				std::exception("Divide By Zero") {}

			/**
			 * \brief Creates a divide by zero exception with a given message
			 * \param message The message of the exception
			 */
			DivideByZero(char const* message) :
				std::exception(message) {}

			/**
			 * \brief Creates a divide by zero exception with a given message
			 * \param message The message of the exception
			 */
			DivideByZero(const std::string& message) :
				std::exception(message.c_str()) {}
		};

		class GimbleLock : public std::exception
		{
		public:
			/**
			 * \brief Creates an invalid index buffer exception with a default message
			 */
			GimbleLock() :
				std::exception("Gimble Lock") {}


			/**
			 * \brief Creates a Gimble Lock exception with a given message
			 * \param message The message of the exception
			 */
			GimbleLock(char const* message) :
				std::exception(message) {}

			/**
			 * \brief Creates a Gimble Lock exception with a given message
			 * \param message The message of the exception
			 */
			GimbleLock(const std::string& message) :
				std::exception(message.c_str()) {}
		};
	}

	class Mesh;

	class Entity
	{
		// Syntactic sugar for shorter lines
		using Mat4 = LibMath::Matrix4;
		using Vec3 = LibMath::Vector3;
		using Rad = LibMath::Radian;

	public:
		#pragma region Constructors
		explicit	Entity(const Mesh& p_mesh, Mat4 p_transform = Mat4()); // TODO : Finish Entity comments
		Entity(const Mesh& p_mesh, float p_transparency, Mat4 p_transform = Mat4());
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

		#pragma region Transforms
		/// <summary>
		/// Get the Mat4 that Scales, Rotates and Translates the Mesh
		/// </summary>
		/// <returns></returns>
		Mat4 getTransform() const;

		/// <summary>
		/// Get the Entity's global position
		/// </summary>
		/// <returns></returns>
		Vec3 getPosition()const;

		/// <summary>
		/// Get the xyz's scaling factors as a Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getScale()const;

		/// <summary>
		/// Get the x axis' scaling factor
		/// </summary>
		/// <returns></returns>
		float getScaleX() const;

		/// <summary>
		/// Get the y axis' scaling factor
		/// </summary>
		/// <returns></returns>
		float getScaleY() const;

		/// <summary>
		/// Get the z axis' scaling factor
		/// </summary>
		/// <returns></returns>
		float getScaleZ() const;

		/// <summary>
		/// Get the Entity's rotation around each axis in radians in a Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getRotationEulerAngles()const;

		/// <summary>
		/// Get the Rightwards, Upwards and Forwards Vectors in a Mat4
		/// </summary>
		/// <returns></returns>
		Mat4 getRotation() const;
		#pragma endregion

		#pragma region LocalDirections
		/// <summary>
		/// Get local rightward Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getRightward()const;

		/// <summary>
		/// Get the inverse of the rightward Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getLeftward()const;

		/// <summary>
		/// Get local upward Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getUpward()const;

		/// <summary>
		/// Get the inverse of the upwards Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getDownward()const;

		/// <summary>
		/// Get local foward Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getForward()const;

		/// <summary>
		/// Get the inverse of the upwards Vector3
		/// </summary>
		/// <returns></returns>
		Vec3 getBackward()const;
#pragma endregion

#pragma endregion

		#pragma region Transformations
		/// <summary>
		/// Replaces transform Mat4
		/// </summary>
		/// <param name="p_transform">: Copy Mat4</param>
		/// <returns></returns>
		void	setTransform(const Mat4& p_transform);

		/// <summary>
		/// Translate Entity's position by paramaters
		/// </summary>
		/// <param name="p_x">: global x axis</param>
		/// <param name="p_y">: global y axis</param>
		/// <param name="p_z">: global z axis</param>
		/// <returns></returns>
		Entity& translate(float p_x, float p_y, float p_z);

		//Entity& translateLocally(float p_x, float p_y, float p_z); // ???

		/// <summary>
		/// Multiply Entity's Scaling factor by paramaters
		/// </summary>
		/// <param name="p_x">: local x axis</param>
		/// <param name="p_y">: local y axis</param>
		/// <param name="p_z">: local z axis</param>
		/// <returns></returns>
		Entity& scale(float p_x, float p_y, float p_z);

		/// <summary>
		/// Rotate Entity around axes by paramaters in Radiants
		/// </summary>
		/// <param name="p_x">: local x axis</param>
		/// <param name="p_y">: local y axis</param>
		/// <param name="p_z">: local z axis</param>
		/// <returns></returns>
		Entity& rotateEulerAngles(const Rad& p_x, const Rad& p_y, const Rad& p_z);

		/// <summary>
		/// Overrides Entity's position with paramaters
		/// </summary>
		/// <param name="p_x">: global x axis</param>
		/// <param name="p_y">: global y axis</param>
		/// <param name="p_z">: global z axis</param>
		/// <returns></returns>
		Entity& setPosition(float p_x, float p_y, float p_z);

		/// <summary>
		/// Overrides Entity's Scaling factor with paramaters
		/// </summary>
		/// <param name="p_x">: local x axis</param>
		/// <param name="p_y">: local y axis</param>
		/// <param name="p_z">: local z axis</param>
		/// <returns></returns>
		Entity& setScale(float p_x, float p_y, float p_z);

		/// <summary>
		/// Overrides Enity's rotation around axes with paramaters in Radiants
		/// </summary>
		/// <param name="p_x">: local x axis</param>
		/// <param name="p_y">: local y axis</param>
		/// <param name="p_z">: local z axis</param>
		/// <returns></returns>
		Entity& setRotationEulerAngles(const Rad& p_x, const Rad& p_y, const Rad& p_z);
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
		Mat4		m_transform;
		float		m_transparency;
		
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

