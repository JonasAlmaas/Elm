#include "renderer_2d.h"

#include "elm/core/renderer/renderer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/core/renderer/vertex_array.h"
#include <glm/gtc/type_ptr.hpp>

namespace elm {

	struct renderer_2d_storage {
		std::shared_ptr<shader> shader;
		std::shared_ptr<texture_2d> texture_blank;
		std::shared_ptr<vertex_array> vertex_array;
	};

	static renderer_2d_storage *s_data;

	void renderer_2d::init(void)
	{
		s_data = new renderer_2d_storage();

		// Load shader
		s_data->shader = elm::shader::create("content/shaders/generic_2d.glsl");
		s_data->shader->bind();
		s_data->shader->set_int("u_texture", 0);

		// Create blank white texture
		s_data->texture_blank = texture_2d::create(1, 1);
		uint8_t t[4] = { 0xFFu, 0xFFu, 0xFFu, 0xFFu };
		s_data->texture_blank->set_data(t, sizeof t);

		// Create vertex array
		s_data->vertex_array = elm::vertex_array::create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f };

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" } };
		vb->set_layout(&layout);
		s_data->vertex_array->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
		s_data->vertex_array->set_index_buffer(ib);
	}

	void renderer_2d::shutdown(void)
	{
		delete s_data;
	}

	void renderer_2d::begin_scene(const orthographic_camera *camera)
	{
		s_data->shader->bind();
		s_data->shader->set_mat4("u_view_projection", camera->get_view_projection_matrix());
	}

	void renderer_2d::end_scene(void)
	{
	}

	static void draw_quad_uber(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size, 1.0f });

		texture->bind();

		s_data->shader->set_mat4("u_transform", transform);
		s_data->shader->set_float4("u_color", color);

		s_data->vertex_array->bind();
		render_command::draw_indexed(s_data->vertex_array);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_uber(position, size, texture, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_uber(glm::vec3(position, 0.0f), size, s_data->texture_blank, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_uber(position, size, s_data->texture_blank, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_uber(glm::vec3(position, 0.0f), size, s_data->texture_blank, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture)
	{
		draw_quad_uber(position, size, texture, glm::vec4(1.0f));
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture)
	{
		draw_quad_uber(glm::vec3(position, 0.0f), size, texture, glm::vec4(1.0f));
	}
}
