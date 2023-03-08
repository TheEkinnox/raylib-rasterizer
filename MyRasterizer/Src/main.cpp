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
constexpr auto ASPECT = static_cast<float>(SCREEN_WIDTH) /
	static_cast<float>(SCREEN_HEIGHT);

using namespace LibMath::Literal;

using Mat4 = LibMath::Matrix4;

int main()
{
	// Initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterize me baby");

	My::Scene scene;
	
	
	My::Mesh* cube = My::Mesh::createCube();
	My::Texture textureWrapper("../img/container.png");
	cube->setTexture(&textureWrapper);

	scene.addMesh("sphereW", *My::Mesh::createSphere(8, 8, My::Color::white));
	scene.addMesh("sphereR", *My::Mesh::createSphere(32, 32, My::Color::red));
	scene.addMesh("sphereG", *My::Mesh::createSphere(32, 32, My::Color::green));
	scene.addMesh("sphereB", *My::Mesh::createSphere(32, 32, My::Color::blue));

	LibMath::Matrix4 transform = LibMath::Matrix4::translation(0, 0, -3) * LibMath::Matrix4::scaling(2, 2, 2);
	scene.addEntity(My::Entity(*cube, transform));

	transform = LibMath::Matrix4::translation(0, 1, -2);
	scene.addEntity(My::Entity(*scene.getMesh("sphereR"), transform));

	transform = LibMath::Matrix4::translation(-1, -1, -2);
	scene.addEntity(My::Entity(*scene.getMesh("sphereG"), transform));

	transform = LibMath::Matrix4::translation(1, -1, -2);
	scene.addEntity(My::Entity(*scene.getMesh("sphereB"), transform));

	//light
	const LibMath::Matrix4 lightScale = LibMath::Matrix4::scaling(.1f, .1f, .1f);

	LibMath::Vector3 lightPos = LibMath::Vector3(0, 2.25f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	lightPos = LibMath::Vector3(-2.25f, -2.25f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	lightPos = LibMath::Vector3(2.25f, -2.25f, 1);
	transform = LibMath::Matrix4::translation(lightPos.m_x, lightPos.m_y, lightPos.m_z) * lightScale;
	scene.addEntity(My::Entity(*scene.getMesh("sphereW"), transform));
	scene.addLight(My::Light(lightPos, 0.1f, 0.5f, 0.4f, 8));

	My::Texture texture(SCREEN_WIDTH, SCREEN_HEIGHT);

	//const Mat4 projMat = Mat4::orthographicProjection(-6, 6, -6, 6, 0.1f, 200.f);
	const Mat4 projMat = Mat4::perspectiveProjection(90_deg, ASPECT, 0.1f, 200.f);

	My::Rasterizer rasterizer;
	rasterizer.renderScene(scene, texture, projMat);

	// Create the texture
	const RenderTexture2D target = LoadRenderTexture(static_cast<int>(texture.getWidth()),
	                                                 static_cast<int>(texture.getHeight()));

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
