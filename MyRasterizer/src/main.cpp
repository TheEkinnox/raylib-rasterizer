#include <raylib.h>

#include "Color.h"
#include "Mesh.h"
#include "Scene.h"
#include "Entity.h"
#include "Rasterizer.h"
#include "Texture.h"
#include "Matrix/Matrix4.h"
#include "Angle/Radian.h"
#include "Angle/Degree.h"
#include "Arithmetic.h"

constexpr auto SCREEN_WIDTH = 800;
constexpr auto SCREEN_HEIGHT = 800;

int main()
{
	// Initialization
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterize me baby");

	My::Scene scene;
	
	My::Mesh* cube = My::Mesh::createCube();
	const char* path = GetWorkingDirectory();
	My::Texture textureWrapper("../img/container.png");
	cube->setTexture(&textureWrapper);
	LibMath::Matrix4 transform = LibMath::Matrix4::translation(0, 0, 2) * LibMath::Matrix4::scaling(2,2,2);
	scene.addEntity(My::Entity(*cube, transform));
	//transformation tests
	/**
	scene.getEntity(0).Translate(0, 1, 0);
	scene.getEntity(1).Translate(0, -1, 0);

	scene.getEntity(0).Scale(2, 1, 2);
	scene.getEntity(1).Scale(2.0f, 2.0f, 1.0f);

	LibMath::Vector3 s1 = scene.getEntity(0).GetScale();
	LibMath::Vector3 s2 = scene.getEntity(1).GetScale();
	
	LibMath::Vector3 vR = scene.getEntity(0).GetRightward();
	LibMath::Vector3 vU = scene.getEntity(0).GetUpward();
	LibMath::Vector3 vF = scene.getEntity(0).GetForward();

	scene.getEntity(0).RotateEulerAngles((LibMath::Radian)0, (LibMath::Radian)0, (LibMath::Radian)(3.1416f / 2));
	vR = scene.getEntity(0).GetRightward();
	vU = scene.getEntity(0).GetUpward();
	vF = scene.getEntity(0).GetForward();

	scene.getEntity(0).Translate(0, 3, 0);
	scene.getEntity(0).SetPosition(0, 0, 0);

	scene.getEntity(0).Scale(3, 3, 3);
	scene.getEntity(0).SetScale(1.0f, 1.0f, 1.0f);
	
	scene.getEntity(0).RotateEulerAngles((LibMath::Radian)(3.1416f/ 4), (LibMath::Radian)0, (LibMath::Radian)(3.1416f / 4));
	LibMath::Vector3 v = scene.getEntity(0).GetRotationEulerAngles();
	scene.getEntity(0).RotateEulerAngles((LibMath::Radian)(-v.m_x), (LibMath::Radian)(-v.m_y), (LibMath::Radian)(-v.m_z));
	LibMath::Vector3 v2 = scene.getEntity(0).GetRotationEulerAngles();

	float pres = 0.001f;
	bool isZero = true;
	isZero = LibMath::abs(v2.m_x) > pres ? false : isZero;
	isZero = LibMath::abs(v2.m_y) > pres ? false : isZero;
	isZero = LibMath::abs(v2.m_z) > pres ? false : isZero;
	*/

	My::Texture texture(SCREEN_WIDTH, SCREEN_HEIGHT);

	My::Rasterizer::renderScene(scene, texture);

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
