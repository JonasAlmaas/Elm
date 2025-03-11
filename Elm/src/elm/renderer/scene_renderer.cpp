#include "scene_renderer.h"

#include "elm/core/renderer/render_command.h"
#include "elm/scene/components.h"
#include "renderer.h"
#include "renderer_2d.h"

namespace elm::scene_renderer {

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		auto &reg = scene->get_reg();

		render_command::set_clear_color(scene->get_clear_color());
		render_command::clear();

		renderer::begin_scene(camera);

		// TODO: Submit stuff here

		// TODO: Render world grid if enabled

		renderer_2d::begin_scene(camera, false);

		auto view = reg.view<transform_component, circle_renderer_component>();
		for (auto entity : view) {
			auto [transform_comp, circle_renderer_comp] = view.get<transform_component, circle_renderer_component>(entity);
			renderer_2d::draw_circle(
				transform_comp.transform,
				circle_renderer_comp.color,
				circle_renderer_comp.radius,
				circle_renderer_comp.thickness,
				circle_renderer_comp.fade);
		}

		renderer_2d::end_scene();

		renderer::end_scene();
	}
}
