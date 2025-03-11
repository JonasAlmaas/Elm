#include "renderer_2d.h"

#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/core/renderer/uniform_buffer.h"
#include "elm/core/renderer/vertex_array.h"
#include "elm/renderer/renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace elm::renderer_2d {

	struct quad_vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;
		int texture_slot;
	};

	struct circle_vertex {
		glm::vec3 world_position;
		glm::vec3 local_position;
		glm::vec4 color;
		float thickness;
		float fade;
	};

	struct renderer_2d_data {
		static const uint32_t max_texture_slots = 32; // TODO: Render Capabilities

		// Used for circles too
		static const uint32_t max_quads = 20000u;
		static const uint32_t max_quad_verticies = max_quads * 4;
		static const uint32_t max_quad_indices = max_quads * 6;

		std::shared_ptr<shader> generic_2d_shader;
		std::shared_ptr<shader> circle_shader;
		std::shared_ptr<texture_2d> texture_blank; // Texture slot 0

		std::shared_ptr<vertex_array> batch_quad_vertex_array;
		std::shared_ptr<vertex_buffer> batch_quad_vertex_buffer;
		uint32_t batch_quad_count = 0;
		quad_vertex *batch_quad_vertex_buf_base = nullptr;
		quad_vertex *batch_quad_vertex_buf_ptr = nullptr;
		std::array<std::shared_ptr<texture_2d>, max_texture_slots> batch_quad_texture_slots;
		uint32_t batch_quad_texture_slot_ix = 1u; // 0 = blank texture

		std::shared_ptr<vertex_array> batch_circle_vertex_array;
		std::shared_ptr<vertex_buffer> batch_circle_vertex_buffer;
		uint32_t batch_circle_count = 0;
		circle_vertex *batch_circle_vertex_buf_base = nullptr;
		circle_vertex *batch_circle_vertex_buf_ptr = nullptr;

		struct statistics stats;
	};

	static renderer_2d_data s_data;

	extern void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		// Load shader
		s_data.generic_2d_shader = elm::shader::create("content/shaders/generic_2d.glsl");
		s_data.circle_shader = elm::shader::create("content/shaders/circle.glsl");

		// Create blank white texture
		s_data.texture_blank = texture_2d::create(1, 1);
		uint8_t t[4] = { 0xFFu, 0xFFu, 0xFFu, 0xFFu };
		s_data.texture_blank->set_data(t, sizeof t);
		s_data.batch_quad_texture_slots[0] = s_data.texture_blank;

		// -- Quad vertex array --
		s_data.batch_quad_vertex_array = elm::vertex_array::create();

		s_data.batch_quad_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_quad_verticies * sizeof(quad_vertex));
		elm::vertex_buffer_layout quad_vb_layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" },
			{ elm::shader_data_type::Float4, "a_color" },
			{ elm::shader_data_type::Int, "a_texture_slot" } };
		s_data.batch_quad_vertex_buffer->set_layout(&quad_vb_layout);
		s_data.batch_quad_vertex_array->add_vertex_buffer(s_data.batch_quad_vertex_buffer);

		s_data.batch_quad_vertex_buf_base = new quad_vertex[renderer_2d_data::max_quad_verticies];

		uint32_t *quad_indices = new uint32_t[renderer_2d_data::max_quad_indices];
		for (uint32_t i = 0u; i < renderer_2d_data::max_quads; ++i) {
			quad_indices[i * 6 + 0] = i * 4 + 0;
			quad_indices[i * 6 + 1] = i * 4 + 1;
			quad_indices[i * 6 + 2] = i * 4 + 2;
			quad_indices[i * 6 + 3] = i * 4 + 0;
			quad_indices[i * 6 + 4] = i * 4 + 2;
			quad_indices[i * 6 + 5] = i * 4 + 3;
		}
		auto quad_ib = elm::index_buffer::create(quad_indices, renderer_2d_data::max_quad_indices);
		s_data.batch_quad_vertex_array->set_index_buffer(quad_ib);
		
		// -- Circle vertex array --
		s_data.batch_circle_vertex_array = elm::vertex_array::create();

		s_data.batch_circle_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_quad_verticies * sizeof(circle_vertex));
		elm::vertex_buffer_layout circle_vb_layout = {
			{ elm::shader_data_type::Float3, "a_world_position" },
			{ elm::shader_data_type::Float3, "a_local_position" },
			{ elm::shader_data_type::Float4, "a_color" },
			{ elm::shader_data_type::Float, "a_thickness" },
			{ elm::shader_data_type::Float, "a_fade" } };
		s_data.batch_circle_vertex_buffer->set_layout(&circle_vb_layout);
		s_data.batch_circle_vertex_array->add_vertex_buffer(s_data.batch_circle_vertex_buffer);

		s_data.batch_circle_vertex_buf_base = new circle_vertex[renderer_2d_data::max_quad_verticies];

		auto circle_ib = elm::index_buffer::create(quad_indices, renderer_2d_data::max_quad_indices);
		s_data.batch_circle_vertex_array->set_index_buffer(circle_ib);

		delete[] quad_indices;
	}

	extern void shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		delete[] s_data.batch_quad_vertex_buf_base;
		s_data.batch_quad_vertex_buf_base = nullptr;
		s_data.batch_quad_vertex_buf_ptr = nullptr;

		delete[] s_data.batch_circle_vertex_buf_base;
		s_data.batch_circle_vertex_buf_base = nullptr;
		s_data.batch_circle_vertex_buf_ptr = nullptr;
	}

	extern void begin_scene(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		renderer::begin_scene(camera);

		s_data.batch_quad_count = 0u;
		s_data.batch_quad_vertex_buf_ptr = s_data.batch_quad_vertex_buf_base;
		s_data.batch_quad_texture_slot_ix = 1u;

		s_data.batch_circle_count = 0u;
		s_data.batch_circle_vertex_buf_ptr = s_data.batch_circle_vertex_buf_base;
	}

	extern void end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		flush();

		renderer::end_scene();
	}

	static void flush_quads(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (s_data.batch_quad_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.batch_quad_vertex_buf_ptr - (uint8_t *)s_data.batch_quad_vertex_buf_base);
		s_data.batch_quad_vertex_buffer->set_data(s_data.batch_quad_vertex_buf_base, data_size);

		for (uint32_t i = 0; i < s_data.batch_quad_texture_slot_ix; ++i) {
			s_data.batch_quad_texture_slots[i]->bind(i);
		}

		s_data.generic_2d_shader->bind();
		s_data.batch_quad_vertex_array->bind();
		render_command::draw_indexed(s_data.batch_quad_vertex_array, s_data.batch_quad_count * 6);

		s_data.batch_quad_count = 0;
		s_data.batch_quad_vertex_buf_ptr = s_data.batch_quad_vertex_buf_base;
		s_data.batch_quad_texture_slot_ix = 1;

		++s_data.stats.draw_calls;
	}

	static void flush_circles(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (s_data.batch_circle_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.batch_circle_vertex_buf_ptr - (uint8_t *)s_data.batch_circle_vertex_buf_base);
		s_data.batch_circle_vertex_buffer->set_data(s_data.batch_circle_vertex_buf_base, data_size);

		s_data.circle_shader->bind();
		s_data.batch_circle_vertex_array->bind();
		render_command::draw_indexed(s_data.batch_circle_vertex_array, s_data.batch_circle_count * 6);

		s_data.batch_circle_count = 0;
		s_data.batch_circle_vertex_buf_ptr = s_data.batch_circle_vertex_buf_base;

		++s_data.stats.draw_calls;
	}

	extern void flush(void)
	{
		flush_quads();
		flush_circles();
	}

	static void draw_quad_transform(
		const glm::mat4 &transform,
		const std::shared_ptr<texture_2d> &texture,
		const std::array<glm::vec2, 4> *uvs,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		int texture_slot = -1;

		for (uint32_t i = 0; i < s_data.batch_quad_texture_slot_ix; ++i) {
			if (s_data.batch_quad_texture_slots[i]->equal(&*texture)) {
				texture_slot = (int)i;
				break;
			}
		}

		if (texture_slot == -1) {
			if (s_data.batch_quad_texture_slot_ix >= renderer_2d_data::max_texture_slots) {
				flush_quads();
			}

			s_data.batch_quad_texture_slots[s_data.batch_quad_texture_slot_ix] = texture;
			texture_slot = s_data.batch_quad_texture_slot_ix++;
		}


		s_data.batch_quad_vertex_buf_ptr->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_data.batch_quad_vertex_buf_ptr->uv = (uvs ? (*uvs)[0] : glm::vec2(0.0f, 0.0f)) * texture_tiling_factor;
		s_data.batch_quad_vertex_buf_ptr->color = color;
		s_data.batch_quad_vertex_buf_ptr->texture_slot = texture_slot;
		++s_data.batch_quad_vertex_buf_ptr;

		s_data.batch_quad_vertex_buf_ptr->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		s_data.batch_quad_vertex_buf_ptr->uv = (uvs ? (*uvs)[1] : glm::vec2(1.0f, 0.0f)) * texture_tiling_factor;
		s_data.batch_quad_vertex_buf_ptr->color = color;
		s_data.batch_quad_vertex_buf_ptr->texture_slot = texture_slot;
		++s_data.batch_quad_vertex_buf_ptr;

		s_data.batch_quad_vertex_buf_ptr->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		s_data.batch_quad_vertex_buf_ptr->uv = (uvs ? (*uvs)[2] : glm::vec2(1.0f, 1.0f)) * texture_tiling_factor;
		s_data.batch_quad_vertex_buf_ptr->color = color;
		s_data.batch_quad_vertex_buf_ptr->texture_slot = texture_slot;
		++s_data.batch_quad_vertex_buf_ptr;

		s_data.batch_quad_vertex_buf_ptr->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
		s_data.batch_quad_vertex_buf_ptr->uv = (uvs ? (*uvs)[3] : glm::vec2(0.0f, 1.0f)) * texture_tiling_factor;
		s_data.batch_quad_vertex_buf_ptr->color = color;
		s_data.batch_quad_vertex_buf_ptr->texture_slot = texture_slot;
		++s_data.batch_quad_vertex_buf_ptr;

		++s_data.batch_quad_count;
		if (s_data.batch_quad_count >= renderer_2d_data::max_quads) {
			flush_quads();
		}

		++s_data.stats.quad_count;
	}

	/// <summary>
	/// Draw axis aligned quad
	/// </summary>
	static void draw_quad_super(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const std::array<glm::vec2, 4> *uvs,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_transform(transform, texture, uvs, texture_tiling_factor, color);
	}

	/// <summary>
	/// Draw rotated quad
	/// </summary>
	static void draw_quad_super_rotated(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const std::array<glm::vec2, 4> *uvs,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });
		draw_quad_transform(transform, texture, uvs, texture_tiling_factor, color);
	}

	extern void draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, s_data.texture_blank, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, s_data.texture_blank, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, texture, nullptr, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	extern void draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, texture, nullptr, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	extern void draw_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_quad_super(position, size, sub_texture->texture, &sub_texture->uvs, glm::vec2(1.0f), color);
	}

	extern void draw_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_quad_super(glm::vec3(position, 0.0f), size, sub_texture->texture, &sub_texture->uvs, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, s_data.texture_blank, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, s_data.texture_blank, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, nullptr, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, texture, nullptr, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	extern void draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, texture, nullptr, { texture_tiling_factor, texture_tiling_factor }, color);
	}

	extern void draw_rotated_quad(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(position, size, rotation_rad, sub_texture->texture, &sub_texture->uvs, glm::vec2(1.0f), color);
	}

	extern void draw_rotated_quad(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_quad_super_rotated(glm::vec3(position, 0.0f), size, rotation_rad, sub_texture->texture, &sub_texture->uvs, glm::vec2(1.0f), color);
	}

	void draw_circle(const glm::mat4 &transform, const glm::vec4 &color, float radius, float thickness, float fade)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.batch_circle_vertex_buf_ptr->local_position = { -1.0f, -1.0f, 0.0f };
		s_data.batch_circle_vertex_buf_ptr->world_position = transform * glm::vec4(-radius, -radius, 0.0f, 1.0f);
		s_data.batch_circle_vertex_buf_ptr->color = color;
		s_data.batch_circle_vertex_buf_ptr->thickness = thickness;
		s_data.batch_circle_vertex_buf_ptr->fade = fade;
		++s_data.batch_circle_vertex_buf_ptr;

		s_data.batch_circle_vertex_buf_ptr->local_position = { 1.0f, -1.0f, 0.0f };
		s_data.batch_circle_vertex_buf_ptr->world_position = transform * glm::vec4(radius, -radius, 0.0f, 1.0f);
		s_data.batch_circle_vertex_buf_ptr->color = color;
		s_data.batch_circle_vertex_buf_ptr->thickness = thickness;
		s_data.batch_circle_vertex_buf_ptr->fade = fade;
		++s_data.batch_circle_vertex_buf_ptr;

		s_data.batch_circle_vertex_buf_ptr->local_position = { 1.0f, 1.0f, 0.0f };
		s_data.batch_circle_vertex_buf_ptr->world_position = transform * glm::vec4(radius, radius, 0.0f, 1.0f);
		s_data.batch_circle_vertex_buf_ptr->color = color;
		s_data.batch_circle_vertex_buf_ptr->thickness = thickness;
		s_data.batch_circle_vertex_buf_ptr->fade = fade;
		++s_data.batch_circle_vertex_buf_ptr;

		s_data.batch_circle_vertex_buf_ptr->local_position = { -1.0f, 1.0f, 0.0f };
		s_data.batch_circle_vertex_buf_ptr->world_position = transform * glm::vec4(-radius, radius, 0.0f, 1.0f);
		s_data.batch_circle_vertex_buf_ptr->color = color;
		s_data.batch_circle_vertex_buf_ptr->thickness = thickness;
		s_data.batch_circle_vertex_buf_ptr->fade = fade;
		++s_data.batch_circle_vertex_buf_ptr;

		++s_data.batch_circle_count;
		if (s_data.batch_circle_count >= renderer_2d_data::max_quads) {
			flush_circles();
		}

		++s_data.stats.quad_count;
	}

	extern struct statistics get_stats(void)
	{
		return s_data.stats;
	}

	extern void reset_stats(void)
	{
		memset(&s_data.stats, 0, sizeof s_data.stats);
	}
}
