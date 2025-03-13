#include "renderer.h"
#include "renderer_2d.h"
#include "scene_renderer.h"
#include "elm/core/renderer/uniform_buffer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/math/math.h"

namespace elm::renderer {

	struct renderer_data {
		struct camera_data {
			glm::mat4 view_projection;
			glm::vec3 position;
		};
		camera_data camera_buffer;
		std::shared_ptr<uniform_buffer> camera_uniform_buffer;

		struct model_data {
			glm::mat4 transform;
		};
		model_data model_buffer;
		std::shared_ptr<uniform_buffer> model_uniform_buffer;
	};

	static struct renderer_data s_data;

	extern void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::init();
		renderer_2d::init();
		scene_renderer::init();

		s_data.camera_uniform_buffer = uniform_buffer::create(sizeof(struct renderer_data::camera_data), 0);
		s_data.model_uniform_buffer = uniform_buffer::create(sizeof(struct renderer_data::model_data), 1);
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

		s_data.camera_buffer.view_projection = camera->get_view_projection();
		math::decompose_transform(
			glm::inverse(camera->get_view()),
			&s_data.camera_buffer.position,
			nullptr,
			nullptr);

		s_data.camera_uniform_buffer->bind();
		s_data.camera_uniform_buffer->set_data((const void *)&s_data.camera_buffer, sizeof s_data.camera_buffer);
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

		s_data.model_buffer.transform = transform;
		s_data.model_uniform_buffer->bind();
		s_data.model_uniform_buffer->set_data((const void *)&s_data.model_buffer, sizeof s_data.model_buffer);

		shader->bind();
		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}
}
