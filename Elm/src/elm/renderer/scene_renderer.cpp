#include "scene_renderer.h"

#include "elm/core/renderer/render_command.h"
#include "renderer.h"
#include "renderer_2d.h"

namespace elm::scene_renderer {

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		render_command::set_clear_color(scene->get_clear_color());
		render_command::clear();

		renderer::begin_scene(camera);

		// TODO: Submit stuff here

		// TODO: Render world grid if enabled

		renderer::end_scene();
	}
}
