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
	scene.addMesh("cubeR", *My::Mesh::createCube(My::Color::white));
	scene.addMesh("cubeG", *My::Mesh::createCube(My::Color::green));
	scene.addMesh("cubeB", *My::Mesh::createCube(My::Color::blue));


	//LibMath::Matrix4 transform = LibMath::Matrix4::translation(-0.5f, 0, 2);
	//scene.addEntity(My::Entity(*scene.getMesh("cube"), transform));

	LibMath::Matrix4 transform = LibMath::Matrix4::translation(-1, 0, 0) * LibMath::Matrix4::rotationEuler(60_deg, 45_deg, 0_deg);
	scene.addEntity(My::Entity(*scene.getMesh("cubeG"), 0.5f, transform));

	transform = LibMath::Matrix4::translation(0, 0, 5) * LibMath::Matrix4::scaling(2, 2, 1);
	scene.addEntity(My::Entity(*scene.getMesh("cubeR"), transform));

	transform = LibMath::Matrix4::translation(1, 0, 0) * LibMath::Matrix4::rotationEuler(60_deg, 45_deg, 0_deg);
	scene.addEntity(My::Entity(*scene.getMesh("cubeB"), 0.5f, transform));

	//light
	scene.addLight(My::Light(LibMath::Vector3(0, 0, -5), 1.0f, 0.0f, 0.0f));

	My::Texture texture(SCREEN_WIDTH, SCREEN_HEIGHT);

	My::Rasterizer rasterizer;
	rasterizer.renderScene(scene, texture);

	// Create the texture
	const RenderTexture2D target = LoadRenderTexture(static_cast<int>(texture.getWidth()), static_cast<int>(texture.getHeight()));

	// Main game loop
	while (!WindowShouldClose())
	{
		// Update

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
