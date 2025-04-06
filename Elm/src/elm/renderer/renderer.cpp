#include "renderer.hpp"
#include "renderer_2d.hpp"
#include "scene_renderer.hpp"
#include "elm/core/renderer/uniform_buffer.hpp"
#include "elm/core/renderer/render_command.hpp"
#include "elm/math/math.hpp"

namespace elm::renderer {

	struct renderer_data {
		struct {
			glm::mat4 view_projection;
			glm::vec3 position;
		} camera;
		std::shared_ptr<uniform_buffer> camera_ub;

		struct {
			glm::mat4 transform;
		} model;
		std::shared_ptr<uniform_buffer> model_ub;
	};

	static struct renderer_data s_data;

	extern void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::init();
		renderer_2d::init();
		scene_renderer::init();

		s_data.camera_ub = uniform_buffer::create(sizeof renderer_data::camera, 0);
		s_data.model_ub = uniform_buffer::create(sizeof renderer_data::model, 1);
	}

	extern void shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		renderer_2d::shutdown();
		scene_renderer::shutdown();
	}

	extern void on_viewport_resize(uint32_t width, uint32_t height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::set_viewport(0, 0, width, height);
	}

	extern void begin_scene(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.camera.view_projection = camera->get_view_projection();
		math::decompose_transform(
			glm::inverse(camera->get_view()),
			&s_data.camera.position,
			nullptr,
			nullptr);

		s_data.camera_ub->bind();
		s_data.camera_ub->set_data((const void *)&s_data.camera, sizeof s_data.camera);
	}

	extern void end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();
	}

	extern void submit(
		const std::shared_ptr<shader> &shader,
		const std::shared_ptr<vertex_array> &vertex_array,
		const glm::mat4 &transform)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.model.transform = transform;

		s_data.model_ub->bind();
		s_data.model_ub->set_data((const void *)&s_data.model, sizeof s_data.model);

		shader->bind();
		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}
}
