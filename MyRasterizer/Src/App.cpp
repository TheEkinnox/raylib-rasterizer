#include "App.h"

#include <raylib.h>

#include "Entity.h"
#include "Mesh.h"
#include "Rasterizer.h"
#include "Scene.h"

namespace My
{
	App::App(const int p_screenWidth, const int p_screenHeight, const char* p_title)
		: m_renderTexture(p_screenWidth, p_screenHeight), m_rasterizer(2)
	{
		// Initialization
		InitWindow(p_screenWidth, p_screenHeight, p_title);

		const float aspect = static_cast<float>(p_screenWidth)
			/ static_cast<float>(p_screenHeight);

		m_camera = Camera(Mat4::scaling(1.f, 1.f, -1.f) * Mat4::translation(0, 0, -2),
			Mat4::perspectiveProjection(90_deg, aspect, 0.1f, 8.f));
	}

	App::~App()
	{
		CloseWindow();
	}

	void App::run()
	{
		createScene();

		// Render the scene a first time (after that, only rendered when it changes)
		m_rasterizer.renderScene(m_scene, m_camera, m_renderTexture);

		// Main game loop
		while (!WindowShouldClose())
		{
			// Draw
			if (checkInput())
				m_rasterizer.renderScene(m_scene, m_camera, m_renderTexture);

			BeginDrawing();

			// Clear the screen
			ClearBackground(::BLANK);

			// Draw our texture
			for (int x = 0; x < static_cast<int>(m_renderTexture.getWidth()); x++)
			{
				for (int y = 0; y < static_cast<int>(m_renderTexture.getHeight()); y++)
				{
					const Color myColor(m_renderTexture.getPixelColor(x, y));
					const ::Color rlColor{ myColor.m_r, myColor.m_g, myColor.m_b, myColor.m_a };

					DrawPixel(x, y, rlColor);
				}
			}

			// End drawing
			EndDrawing();
		}
	}

	void App::createScene()
	{
		m_scene = Scene();
		m_camera.setTransform(Mat4::scaling(1.f, 1.f, -1.f) * Mat4::translation(0, 0, 2));

		Mesh* cube = Mesh::createCube();
		cube->setTexture(&CONTAINER_TEXTURE);
		m_scene.addMesh("cube", *cube);

		m_scene.addMesh("sphereW", *Mesh::createSphere(4, 4, Color::white));
		m_scene.addMesh("sphereR", *Mesh::createSphere(16, 16, Color::red));
		m_scene.addMesh("sphereG", *Mesh::createSphere(16, 16, Color::green));
		m_scene.addMesh("sphereB", *Mesh::createSphere(16, 16, Color::blue));

		Mat4 transform = Mat4::translation(0, 0, 2) * Mat4::scaling(1.25f, 1.25f, 1.25f);
		m_scene.addEntity(Entity(*m_scene.getMesh("cube"), 0.4f, transform));

		transform = Mat4::translation(0, 1.f, 3);
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereR"), transform));

		transform = Mat4::translation(-1.f, -1.f, 3);
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereG"), transform));

		transform = Mat4::translation(1.f, -1.f, 3);
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereB"), transform));

		//light
		const Mat4 lightScale = Mat4::scaling(.05f, .05f, .05f);

		Vec3 lightPos = Vec3(0, 2.f, 1);
		transform = Mat4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereW"), transform));
		m_scene.addLight(Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

		lightPos = Vec3(-2.f, -2.f, 1);
		transform = Mat4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereW"), transform));
		m_scene.addLight(Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

		lightPos = Vec3(2.f, -2.f, 1);
		transform = Mat4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
		m_scene.addEntity(Entity(*m_scene.getMesh("sphereW"), transform));
		m_scene.addLight(Light(lightPos, 0.1f, 0.5f, 0.4f, 8));
	}

	bool App::checkInput()
	{
		const float deltaTime = GetFrameTime();
		bool hasSceneChanged = false;

		if (IsKeyPressed(KEY_C))
		{
			// Reset scene
			createScene();
			return true;
		}

		if (IsKeyPressed(KEY_F1))
		{
			m_rasterizer.toggleWireFrameMode();
			hasSceneChanged = true;
		}

		if (IsKeyDown(KEY_R))
		{
			for (auto& entity : m_scene.getEntities())
				entity.rotateEulerAngles(ROTATION_SPEED * deltaTime, ROTATION_SPEED * deltaTime, 0_rad);

			hasSceneChanged = true;
		}

		if (IsKeyDown(KEY_W))
		{
			m_camera.translate(m_camera.getForward() * MOVE_SPEED * deltaTime);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_S))
		{
			m_camera.translate(m_camera.getBackward() * MOVE_SPEED * deltaTime);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_Q))
		{
			m_camera.translate(m_camera.getLeftward() * MOVE_SPEED * deltaTime);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_E))
		{
			m_camera.translate(m_camera.getRightward() * MOVE_SPEED * deltaTime);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			m_camera.rotateEulerAngles(0_rad, ROTATION_SPEED * deltaTime, 0_rad);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			m_camera.rotateEulerAngles(0_rad, -ROTATION_SPEED * deltaTime, 0_rad);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_UP))
		{
			m_camera.rotateEulerAngles(ROTATION_SPEED * deltaTime, 0_rad, 0_rad);
			hasSceneChanged = true;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			m_camera.rotateEulerAngles(-ROTATION_SPEED * deltaTime, 0_rad, 0_rad);
			hasSceneChanged = true;
		}

		return hasSceneChanged;
	}
}
