#include "scene_renderer.h"

#include "elm/core/renderer/render_command.h"
#include "renderer.h"
#include "renderer_2d.h"

namespace elm::scene_renderer {

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		render_command::set_clear_color({ 1.0f, 0.0f, 1.0f, 1.0f });
		render_command::clear();

		renderer::begin_scene(camera);

		// Todo: Submit stuff here

		renderer::end_scene();
	}
}
