#pragma once

namespace My
{
	class Texture;
	class Scene;

	class Rasterizer
	{
	public:
		Rasterizer() = default;
		~Rasterizer() = default;

		/**
		 * \brief Draws the scene in 3D in the target texture
		 * \param p_scene The scene to render
		 * \param p_target The texture to which the scene should be rendered
		 */
		static void renderScene(const Scene& p_scene, Texture& p_target);
	};
}