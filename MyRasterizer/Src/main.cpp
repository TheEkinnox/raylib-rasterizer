#include <raylib.h>

#include "Color.h"
#include "Mesh.h"
#include "Scene.h"
#include "Entity.h"
#include "Rasterizer.h"
#include "Texture.h"
#include "Matrix/Matrix4.h"
#include "Angle.h"
#include "Camera.h"

using namespace LibMath::Literal;
using Mat4 = LibMath::Matrix4;

constexpr auto			SCREEN_WIDTH = 800;
constexpr auto			SCREEN_HEIGHT = 600;
constexpr auto			ASPECT = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);

constexpr float			MOVE_SPEED = 1.f;

const LibMath::Degree	g_rotationSpeed = 10_deg;
const My::Texture		g_containerTexture("../img/container.png");

My::Scene createScene()
{
	My::Scene scene;

	My::Mesh* cube = My::Mesh::createCube();
	cube->setTexture(&g_containerTexture);
	scene.addMesh("cube", *cube);

	scene.addMesh("sphereW", *My::Mesh::createSphere(8, 8, My::Color::white));
	scene.addMesh("sphereR", *My::Mesh::createSphere(16, 16, My::Color::red));
	scene.addMesh("sphereG", *My::Mesh::createSphere(16, 16, My::Color::green));
	scene.addMesh("sphereB", *My::Mesh::createSphere(16, 16, My::Color::blue));

	LibMath::Matrix4 transform = LibMath::Matrix4::translation(0, 0, 2) * LibMath::Matrix4::scaling(1.25f, 1.25f, 1.25f);
	scene.addEntity(My::Entity(*scene.getMesh("cube"), 0.25f, transform));

	transform = LibMath::Matrix4::translation(0, 1.f, 3);
	scene.addEntity(My::Entity(*scene.getMesh("sphereR"), transform));

	transform = LibMath::Matrix4::translation(-1.f, -1.f, 3);
	scene.addEntity(My::Entity(*scene.getMesh("sphereG"), transform));

	transform = LibMath::Matrix4::translation(1.f, -1.f, 3);
	scene.addEntity(My::Entity(*scene.getMesh("sphereB"), transform));

	//light
	const LibMath::Matrix4 lightScale = LibMath::Matrix4::scaling(.05f, .05f, .05f);

	LibMath::Vector3 lightPos = LibMath::Vector3(0, 2.f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	lightPos = LibMath::Vector3(-2.f, -2.f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	lightPos = LibMath::Vector3(2.f, -2.f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	return scene;
}

bool checkInput(My::Rasterizer& rasterizer, My::Camera& camera, My::Scene& scene)
{
	const float deltaTime = GetFrameTime();
	bool hasSceneChanged = false;

	if (IsKeyPressed(KEY_C))
	{
		// Reset camera's transform
		camera.setTransform(Mat4::scaling(1.f, 1.f, -1.f));

		// Reset scene
		scene = createScene();

		return true;
	}

	if (IsKeyPressed(KEY_F1))
	{
		rasterizer.toggleWireFrameMode();
		hasSceneChanged = true;
	}

	if (IsKeyDown(KEY_R))
	{
		for (auto& entity : scene.getEntities())
			entity.rotateEulerAngles(g_rotationSpeed * deltaTime, g_rotationSpeed * deltaTime, 0_rad);

		hasSceneChanged = true;
	}

	if (IsKeyDown(KEY_W))
	{
		camera.translate(camera.getForward() * MOVE_SPEED * deltaTime);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_S))
	{
		camera.translate(camera.getBackward() * MOVE_SPEED * deltaTime);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_Q))
	{
		camera.translate(camera.getLeftward() * MOVE_SPEED * deltaTime);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_E))
	{
		camera.translate(camera.getRightward() * MOVE_SPEED * deltaTime);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		camera.rotateEulerAngles(0_rad, g_rotationSpeed * deltaTime, 0_rad);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		camera.rotateEulerAngles(0_rad, -g_rotationSpeed * deltaTime, 0_rad);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_UP))
	{
		camera.rotateEulerAngles(g_rotationSpeed * deltaTime, 0_rad, 0_rad);
		hasSceneChanged = true;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		camera.rotateEulerAngles(-g_rotationSpeed * deltaTime, 0_rad, 0_rad);
		hasSceneChanged = true;
	}

	return hasSceneChanged;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterize me baby");

	My::Scene scene = createScene();

	//const Mat4 projMat = Mat4::orthographicProjection(-6, 6, -6, 6, 0.1f, 200.f);
	const Mat4 projMat = Mat4::perspectiveProjection(90_deg, ASPECT, 0.1f, 5.f);

	My::Camera camera(Mat4::scaling(1.f, 1.f, -1.f), projMat);

	My::Rasterizer rasterizer(1);

	// Create the render textures
	My::Texture texture(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Render the scene a first time (after that, only rendered when it changes)
	rasterizer.renderScene(scene, camera, texture);

	// Main game loop
	while (!WindowShouldClose())
	{
		// Draw
		if (checkInput(rasterizer, camera, scene))
			rasterizer.renderScene(scene, camera, texture);

		BeginDrawing();

		// Clear the screen
		ClearBackground(BLANK);

		// Draw our texture
		for (int x = 0; x < static_cast<int>(texture.getWidth()); x++)
		{
			for (int y = 0; y < static_cast<int>(texture.getHeight()); y++)
			{
				const My::Color myColor(texture.getPixelColor(x, y));
				const Color rlColor{ myColor.m_r, myColor.m_g, myColor.m_b, myColor.m_a };

				DrawPixel(x, y, rlColor);
			}
		}

		// End drawing
		EndDrawing();
	}

	// Cleanup
	CloseWindow();

	_CrtDumpMemoryLeaks();

	return 0;
}
