#include <raylib.h>

constexpr auto SCREEN_WIDTH = 1280;
constexpr auto SCREEN_HEIGHT = 720;

int main()
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterize me baby");

    // Create the texture
    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();

        // Set the render target
        BeginTextureMode(target);

        // Clear the screen
        ClearBackground(BLUE);

        // Draw a vertical line of yellow dots
        for (int i = 0; i < SCREEN_HEIGHT; i += 4)
        {
            DrawPixel(SCREEN_WIDTH / 2, i, YELLOW);
        }

        // Reset the render target
        EndTextureMode();

        // Draw the render texture to the screen
        DrawTexturePro(target.texture, { 0, 0, (float)target.texture.width,
                                       -(float)target.texture.height },
                                       { 0, 0, (float)GetScreenWidth(),
                                       (float)GetScreenHeight() },
                                       { 0, 0 }, 0, WHITE);

        // End drawing
        EndDrawing();
    }

    // Cleanup
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
