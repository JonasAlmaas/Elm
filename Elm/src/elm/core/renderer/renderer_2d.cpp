#include "renderer_2d.h"

#include "elm/core/renderer/renderer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/vertex_array.h"
#include <glm/gtc/type_ptr.hpp>

// Temporary
#include "elm/platform/opengl/opengl_shader.h"

namespace elm {

	struct renderer_2d_storage {
		std::shared_ptr<shader> flat_color_shader;
		std::shared_ptr<vertex_array> vertex_array;
	};

	static renderer_2d_storage *s_data;

	void renderer_2d::init(void)
	{
		s_data = new renderer_2d_storage();

		s_data->vertex_array = elm::vertex_array::create();

		float vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f };

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" } };
		vb->set_layout(&layout);
		s_data->vertex_array->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
		s_data->vertex_array->set_index_buffer(ib);

		s_data->flat_color_shader = elm::shader::create("content/shaders/flat_color.glsl");
	}

	void renderer_2d::shutdown(void)
	{
		delete s_data;
	}

	void renderer_2d::begin_scene(const orthographic_camera *camera)
	{
		s_data->flat_color_shader->bind();
		std::dynamic_pointer_cast<opengl_shader>(s_data->flat_color_shader)->upload_uniform_mat4("u_view_projection", camera->get_view_projection_matrix());
	}

	void renderer_2d::end_scene(void)
	{
	}

	void renderer_2d::draw_quad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_data->flat_color_shader->bind();
		std::dynamic_pointer_cast<elm::opengl_shader>(s_data->flat_color_shader)->upload_uniform_float4("u_color", color);
		std::dynamic_pointer_cast<opengl_shader>(s_data->flat_color_shader)->upload_uniform_mat4("u_transform", transform);

		s_data->vertex_array->bind();
		render_command::draw_indexed(s_data->vertex_array);
	}

	void renderer_2d::draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		draw_quad(glm::vec3(position, 0.0f), size, color);
	}
}
