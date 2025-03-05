#include "renderer_2d.h"

#include "elm/core/renderer/renderer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/core/renderer/vertex_array.h"
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace elm {

	struct quad_vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;
		float texture_slot;
	};

	struct renderer_2d_data {
		static const uint32_t max_quads = 20000u;
		static const uint32_t max_verticies = max_quads * 4;
		static const uint32_t max_indices = max_quads * 6;
		static const uint32_t max_texture_slots = 32; // TODO: Render Capabilities

		std::shared_ptr<shader> shader;
		std::shared_ptr<texture_2d> texture_blank; // Texture slot 0

		std::shared_ptr<vertex_array> quad_vertex_array;
		std::shared_ptr<vertex_buffer> quad_vertex_buffer;

		uint32_t quad_count = 0;
		quad_vertex *quad_vertex_buf_base = nullptr;
		quad_vertex *quad_vertex_buf_ptr = nullptr;

		std::array<std::shared_ptr<texture_2d>, max_texture_slots> texture_slots;
		uint32_t texture_slot_ix = 1u; // 0 = blank texture
	};

	static renderer_2d_data s_data;

	void renderer_2d::init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		// Load shader
		s_data.shader = elm::shader::create("content/shaders/generic_2d.glsl");
		s_data.shader->bind();
		int sampler[renderer_2d_data::max_texture_slots];
		for (int i = 0; i < renderer_2d_data::max_texture_slots; ++i) {
			sampler[i] = i;
		}
		s_data.shader->set_int_array("u_textures", sampler, renderer_2d_data::max_texture_slots);

		// Create blank white texture
		s_data.texture_blank = texture_2d::create(1, 1);
		uint8_t t[4] = { 0xFFu, 0xFFu, 0xFFu, 0xFFu };
		s_data.texture_blank->set_data(t, sizeof t);
		s_data.texture_slots[0] = s_data.texture_blank;

		// -- Quad vertex array --
		s_data.quad_vertex_array = elm::vertex_array::create();

		s_data.quad_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_verticies * sizeof(quad_vertex));
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" },
			{ elm::shader_data_type::Float4, "a_color" },
			{ elm::shader_data_type::Float, "a_texture_slot" } };
		s_data.quad_vertex_buffer->set_layout(&layout);
		s_data.quad_vertex_array->add_vertex_buffer(s_data.quad_vertex_buffer);

		s_data.quad_vertex_buf_base = new quad_vertex[renderer_2d_data::max_verticies];

		uint32_t *quad_indices = new uint32_t[renderer_2d_data::max_indices];
		for (uint32_t i = 0u; i < renderer_2d_data::max_quads; ++i) {
			quad_indices[i * 6 + 0] = i * 4 + 0;
			quad_indices[i * 6 + 1] = i * 4 + 1;
			quad_indices[i * 6 + 2] = i * 4 + 2;
			quad_indices[i * 6 + 3] = i * 4 + 0;
			quad_indices[i * 6 + 4] = i * 4 + 2;
			quad_indices[i * 6 + 5] = i * 4 + 3;
		}
		auto quad_ib = elm::index_buffer::create(quad_indices, renderer_2d_data::max_indices);
		s_data.quad_vertex_array->set_index_buffer(quad_ib);
		delete[] quad_indices;
	}

	void renderer_2d::shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		delete[] s_data.quad_vertex_buf_base;
		s_data.quad_vertex_buf_base = nullptr;
		s_data.quad_vertex_buf_ptr = nullptr;
	}

	void renderer_2d::begin_scene(const orthographic_camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.shader->bind();
		s_data.shader->set_mat4("u_view_projection", camera->get_view_projection_matrix());

		s_data.texture_slots[0]->bind(0); // Blank texture

		s_data.quad_count = 0u;
		s_data.quad_vertex_buf_ptr = s_data.quad_vertex_buf_base;
		s_data.texture_slot_ix = 1u;
	}

	void renderer_2d::end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		flush();
	}

	void renderer_2d::flush(void)
	{
		if (s_data.quad_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.quad_vertex_buf_ptr - (uint8_t *)s_data.quad_vertex_buf_base);
		s_data.quad_vertex_buffer->set_data(s_data.quad_vertex_buf_base, data_size);

		// First slot is the blank texture, bound in begin_scene()
		for (uint32_t i = 1u; i < s_data.texture_slot_ix; ++i) {
			s_data.texture_slots[i]->bind(i);
		}

		s_data.quad_vertex_array->bind();
		render_command::draw_indexed(s_data.quad_vertex_array, s_data.quad_count * 6);

		s_data.quad_count = 0;
		s_data.quad_vertex_buf_ptr = s_data.quad_vertex_buf_base;
		s_data.texture_slot_ix = 1;
	}

	void renderer_2d::draw_quad_transform(
		const glm::mat4 &transform,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		int texture_slot = -1;

		for (uint32_t i = 0; i < s_data.texture_slot_ix; ++i) {
			if (s_data.texture_slots[i]->equal(&*texture)) {
				texture_slot = (int)i;
				break;
			}
		}

		if (texture_slot == -1) {
			if (s_data.texture_slot_ix >= renderer_2d_data::max_texture_slots) {
				flush();
			}

			s_data.texture_slots[s_data.texture_slot_ix] = texture;
			texture_slot = s_data.texture_slot_ix++;
		}

		s_data.quad_vertex_buf_ptr->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_data.quad_vertex_buf_ptr->uv = glm::vec2(0.0f, 0.0f) * texture_tiling_factor;
		s_data.quad_vertex_buf_ptr->color = color;
		s_data.quad_vertex_buf_ptr->texture_slot = (float)texture_slot;
		++s_data.quad_vertex_buf_ptr;

		s_data.quad_vertex_buf_ptr->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		s_data.quad_vertex_buf_ptr->uv = glm::vec2(1.0f, 0.0f) * texture_tiling_factor;
		s_data.quad_vertex_buf_ptr->color = color;
		s_data.quad_vertex_buf_ptr->texture_slot = (float)texture_slot;
		++s_data.quad_vertex_buf_ptr;

		s_data.quad_vertex_buf_ptr->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		s_data.quad_vertex_buf_ptr->uv = glm::vec2(1.0f, 1.0f) * texture_tiling_factor;
		s_data.quad_vertex_buf_ptr->color = color;
		s_data.quad_vertex_buf_ptr->texture_slot = (float)texture_slot;
		++s_data.quad_vertex_buf_ptr;

		s_data.quad_vertex_buf_ptr->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		s_data.quad_vertex_buf_ptr->uv = glm::vec2(0.0f, 1.0f) * texture_tiling_factor;
		s_data.quad_vertex_buf_ptr->color = color;
		s_data.quad_vertex_buf_ptr->texture_slot = (float)texture_slot;
		++s_data.quad_vertex_buf_ptr;

		++s_data.quad_count;
		if (s_data.quad_count >= renderer_2d_data::max_quads) {
			flush();
		}
	}

	/// <summary>
	/// Draw axis aligned quad
	/// </summary>
	void renderer_2d::draw_quad_super(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_transform(transform, texture, texture_tiling_factor, color);
	}

	/// <summary>
	/// Draw rotated quad
	/// </summary>
	void renderer_2d::draw_quad_super_rotated(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_transform(transform, texture, texture_tiling_factor, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, s_data.texture_blank, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, s_data.texture_blank, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	void renderer_2d::draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, s_data.texture_blank, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, s_data.texture_blank, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, glm::vec2(1.0f), color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	void renderer_2d::draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, { texture_tiling_factor, texture_tiling_factor }, color);
	}
}
