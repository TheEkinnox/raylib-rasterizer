#include "Rasterizer.h"

#include "Color.h"
#include "Texture.h"
#include "Scene.h"

namespace My
{
	void Rasterizer::renderScene(const Scene& p_scene, Texture& p_target)
	{
		// Set every pixel to black
		for (uint32_t x = 0; x < p_target.getWidth(); x++)
			for (uint32_t y = 0; y < p_target.getWidth(); y++)
				p_target.setPixelColor(x, y, Color::BLACK);

		// TODO: Rasterize scene entities
	}
}
