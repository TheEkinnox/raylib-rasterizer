#include <raylib.h>

#include "App.h"

constexpr auto	SCREEN_WIDTH = 800;
constexpr auto	SCREEN_HEIGHT = 600;

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	{
		My::App app(SCREEN_WIDTH, SCREEN_HEIGHT, "Rasterizer v0.10");

		app.run();
	}

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}
