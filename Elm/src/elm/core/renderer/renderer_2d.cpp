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
		ELM_PROFILE_RENDERER_FUNCTION();

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
		ELM_PROFILE_RENDERER_FUNCTION();

		delete s_data;
	}

	void renderer_2d::begin_scene(const orthographic_camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data->shader->bind();
		s_data->shader->set_mat4("u_view_projection", camera->get_view_projection_matrix());
	}

	void renderer_2d::end_scene(void)
	{
	}

	static void draw_quad_uber(
		const glm::mat4 &transform,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		texture->bind();

		s_data->shader->set_mat4("u_transform", transform);
		s_data->shader->set_float4("u_color", color);
		s_data->shader->set_float("u_texture_tiling_factor", texture_tiling_factor);

		s_data->vertex_array->bind();
		render_command::draw_indexed(s_data->vertex_array);
	}

	/// <summary>
	/// Draw axis aligned quad
	/// </summary>
	static void draw_quad_super(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_uber(transform, texture, texture_tiling_factor, color);
	}

	/// <summary>
	/// Draw rotated quad
	/// </summary>
	static void draw_quad_super_rotated(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_uber(transform, texture, texture_tiling_factor, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, s_data->texture_blank, 1.0f, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, s_data->texture_blank, 1.0f, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, 1.0f, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, 1.0f, color);
	}	

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, texture_tiling_factor, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, texture_tiling_factor, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, s_data->texture_blank, 1.0f, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, s_data->texture_blank, 1.0f, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, 1.0f, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, 1.0f, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, texture_tiling_factor, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, texture_tiling_factor, color);
	}
}
