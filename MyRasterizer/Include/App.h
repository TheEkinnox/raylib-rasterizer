#pragma once
#include "Matrix/Matrix4.h"
#include "Rasterizer.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"
#include "Angle/Degree.h"

namespace My
{
	using namespace LibMath::Literal;

	class App
	{
		using Mat4 = LibMath::Matrix4;
		using Vec3 = LibMath::Vector3;

	public:
		/**
		 * \brief Initializes the application window and other necessary data
		 * \param p_screenWidth The width of the application window
		 * \param p_screenHeight The height of the application window
		 * \param p_title The title of the application window
		 */
		App(int p_screenWidth, int p_screenHeight, const char* p_title);

		/**
		 * \brief Closes the application window and cleans the loaded data
		 */
		~App();

		/**
		 * \brief Runs the application's main loop
		 */
		void run();

	private:
		const float				MOVE_SPEED = 1.f;
		const LibMath::Degree	ROTATION_SPEED = 10_deg;
		const Texture			CONTAINER_TEXTURE = Texture("../img/container.png");

		Texture		m_renderTexture;
		Rasterizer	m_rasterizer;
		Camera		m_camera;
		Scene		m_scene;

		/**
		 * \brief Creates or resets the scene to render
		 */
		void createScene();

		/**
		 * \brief Handles user's key presses
		 * then returns whether the scene should be redrawn or not
		 * \return True if the scene has changed. False otherwise
		 */
		bool checkInput();
	};
}
