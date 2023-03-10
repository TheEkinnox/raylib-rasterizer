#include <raylib.h>

#include "Color.h"
#include "Mesh.h"
#include "Scene.h"
#include "Entity.h"
#include "Rasterizer.h"
#include "Texture.h"
#include "Matrix/Matrix4.h"
#include "Angle.h"

constexpr auto SCREEN_WIDTH = 800;
constexpr auto SCREEN_HEIGHT = 600;

using namespace LibMath::Literal;

int main()
{
	// Initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterize me baby");

	My::Scene scene;
	//scene.addMesh("cube", *My::Mesh::createCube());
	scene.addMesh("cubeW", *My::Mesh::createCube(My::Color::white));
	scene.addMesh("cubeR", *My::Mesh::createCube(My::Color::red));
	scene.addMesh("cubeG", *My::Mesh::createCube({ 0, 255, 0, 32 }));
	scene.addMesh("cubeB", *My::Mesh::createCube(My::Color::blue));

	scene.addMesh("sphere", *My::Mesh::createSphere(8,8, My::Color::blue));
	//scene.addMesh("sphere", *My::Mesh::createSphere(8,8, My::Color(UINT8_MAX, UINT8_MAX, UINT8_MAX, 127)));

	std::vector<My::Vertex> vertex
	{
		{{-1,2,0},{0,0,-1}, My::Color::white},
		{{2,1,0},{0,0,-1}, My::Color::white},
		{{-0,-2,0},{0,0,-1}, My::Color::white},
		{{4,-3,0},{0,0,-1}, My::Color::white},
	};

	std::vector<size_t> indices
	{
		0, 2, 1,
		1, 2, 3
	};

	//LibMath::Matrix4 transform = LibMath::Matrix4::translation(-0.5f, 0, 2);
	//scene.addEntity(My::Entity(*scene.getMesh("cube"), transform));

	LibMath::Matrix4 transform = LibMath::Matrix4::translation(0, 0, 0);
	scene.addEntity(My::Entity(*scene.getMesh("sphere"), 0.5f, transform));

	transform = LibMath::Matrix4::translation(0, 0, 4);
	scene.addEntity(My::Entity(*scene.getMesh("cubeW"), 1.f, transform));

	transform = LibMath::Matrix4::translation(0.5f, 0, 5) * LibMath::Matrix4::rotationEuler(60_deg, 45_deg, 0_deg);
	scene.addEntity(My::Entity(*scene.getMesh("cubeR"), 0.2f, transform));

	transform = LibMath::Matrix4::translation(0, 0, 8) * LibMath::Matrix4::rotationEuler(0_deg, 10_deg, 45_deg);
	scene.addEntity(My::Entity(*scene.getMesh("cubeG"), 1.f, transform));

	//transform = LibMath::Matrix4::translation(0, 0, 0) * LibMath::Matrix4::rotationEuler(60_deg, 45_deg, 0_deg);
	//scene.addEntity(My::Entity(*scene.getMesh("cubeR"), 0.2f, transform));
	//

	//LibMath::Matrix4 transform = LibMath::Matrix4::translation(0,0,0);
	//scene.addEntity(My::Entity(*scene.getMesh("square"), transform));

	//light
	scene.addLight(My::Light(LibMath::Vector3(0, 0, -10), 0.2f, 0.4f, 0.4f));



	My::Texture texture(SCREEN_WIDTH, SCREEN_HEIGHT);

	My::Rasterizer rasterizer;
	rasterizer.renderScene(scene, texture);

	// Create the texture
	const RenderTexture2D target = LoadRenderTexture(static_cast<int>(texture.getWidth()), static_cast<int>(texture.getHeight()));

	// Main game loop
	while (!WindowShouldClose())
	{
		// Update
		if (IsKeyPressed(KEY_F1))
		{
			rasterizer.toggleWireFrameMode();
			rasterizer.renderScene(scene, texture);
		}

		// Draw
		BeginDrawing();

		// Set the render target
		BeginTextureMode(target);

		// Clear the screen
		ClearBackground(BLANK);

		// Draw our texture
		for (int x = 0; x < static_cast<int>(texture.getWidth()); x++)
		{
			for (int y = 0; y < static_cast<int>(texture.getHeight()); y++)
			{
				const My::Color myColor(texture.getPixelColor(x, y));
				const Color rlColor{myColor.m_r, myColor.m_g, myColor.m_b, myColor.m_a};

				DrawPixel(x, y, rlColor);
			}
		}

		// Reset the render target
		EndTextureMode();

		// Draw the render texture to the screen
		DrawTexturePro(target.texture, {
			               0, 0, static_cast<float>(target.texture.width),
			               -static_cast<float>(target.texture.height)
		               },
		               {
			               0, 0, static_cast<float>(GetScreenWidth()),
			               static_cast<float>(GetScreenHeight())
		               },
		               {0, 0}, 0, WHITE);

		// End drawing
		EndDrawing();
	}

	// Cleanup
	UnloadRenderTexture(target);
	CloseWindow();

	return 0;
}
