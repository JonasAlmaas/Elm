#include "renderer_2d.h"

#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/core/renderer/uniform_buffer.h"
#include "elm/core/renderer/vertex_array.h"
#include <glm/gtc/type_ptr.hpp>
#include <array>

namespace elm::renderer_2d {

	struct sprite_vertex {
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

	struct line_vertex {
		glm::vec3 position;
		glm::vec4 color;
	};
	
	struct renderer_2d_data {
		static const uint32_t max_texture_slots = 32; // TODO: Render Capabilities

		// Used for circles and lines too
		static const uint32_t max_sprites = 20000u;
		static const uint32_t max_sprite_verticies = max_sprites * 4;
		static const uint32_t max_sprite_indices = max_sprites * 6;

		struct camera_data {
			glm::mat4 view_projection;
		};
		camera_data camera_buffer;
		std::shared_ptr<uniform_buffer> camera_uniform_buffer;

		std::shared_ptr<shader> sprite_shader;
		std::shared_ptr<shader> circle_shader;
		std::shared_ptr<shader> line_shader;

		std::shared_ptr<texture_2d> texture_blank; // Texture slot 0

		// Sprite
		std::shared_ptr<vertex_array> batch_sprite_vertex_array;
		std::shared_ptr<vertex_buffer> batch_sprite_vertex_buffer;
		uint32_t batch_sprite_count = 0;
		sprite_vertex *batch_sprite_vertex_buf_base = nullptr;
		sprite_vertex *batch_sprite_vertex_buf_ptr = nullptr;
		std::array<std::shared_ptr<texture_2d>, max_texture_slots> batch_sprite_texture_slots;
		uint32_t batch_sprite_texture_slot_ix = 1u; // 0 = blank texture

		// Circle
		std::shared_ptr<vertex_array> batch_circle_vertex_array;
		std::shared_ptr<vertex_buffer> batch_circle_vertex_buffer;
		uint32_t batch_circle_count = 0;
		circle_vertex *batch_circle_vertex_buf_base = nullptr;
		circle_vertex *batch_circle_vertex_buf_ptr = nullptr;

		// Line
		float line_thickness = 2.0f;
		std::shared_ptr<vertex_array> batch_line_vertex_array;
		std::shared_ptr<vertex_buffer> batch_line_vertex_buffer;
		uint32_t batch_line_count = 0;
		line_vertex *batch_line_vertex_buf_base = nullptr;
		line_vertex *batch_line_vertex_buf_ptr = nullptr;

		struct statistics stats;
	};

	static renderer_2d_data s_data;

	static std::array<glm::vec4, 4> s_quad_vertices = {
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
		glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
	};

	static std::array<glm::vec2, 4> s_quad_uvs = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
	};

	extern void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.camera_uniform_buffer = uniform_buffer::create(sizeof(struct renderer_2d_data::camera_data), 0);

		// Load shaders
		s_data.sprite_shader = elm::shader::create("content/shaders/sprite.glsl");
		s_data.circle_shader = elm::shader::create("content/shaders/circle_unlit.glsl");
		s_data.line_shader = elm::shader::create("content/shaders/line.glsl");

		// Create blank white texture
		s_data.texture_blank = texture_2d::create(1, 1);
		uint32_t t = 0xFFFFFFFFu;
		s_data.texture_blank->set_data(&t, sizeof t);
		s_data.batch_sprite_texture_slots[0] = s_data.texture_blank;

		// -- Quad vertex array --
		s_data.batch_sprite_vertex_array = elm::vertex_array::create();

		s_data.batch_sprite_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(sprite_vertex));
		elm::vertex_buffer_layout quad_vb_layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" },
			{ elm::shader_data_type::Float4, "a_color" },
			{ elm::shader_data_type::Int, "a_texture_slot" } };
		s_data.batch_sprite_vertex_buffer->set_layout(&quad_vb_layout);
		s_data.batch_sprite_vertex_array->add_vertex_buffer(s_data.batch_sprite_vertex_buffer);

		s_data.batch_sprite_vertex_buf_base = new sprite_vertex[renderer_2d_data::max_sprite_verticies];

		uint32_t *quad_indices = new uint32_t[renderer_2d_data::max_sprite_indices];
		for (uint32_t i = 0u; i < renderer_2d_data::max_sprites; ++i) {
			quad_indices[i * 6 + 0] = i * 4 + 0;
			quad_indices[i * 6 + 1] = i * 4 + 1;
			quad_indices[i * 6 + 2] = i * 4 + 2;
			quad_indices[i * 6 + 3] = i * 4 + 0;
			quad_indices[i * 6 + 4] = i * 4 + 2;
			quad_indices[i * 6 + 5] = i * 4 + 3;
		}
		auto quad_ib = elm::index_buffer::create(quad_indices, renderer_2d_data::max_sprite_indices);
		s_data.batch_sprite_vertex_array->set_index_buffer(quad_ib);

		// -- Circle vertex array --
		s_data.batch_circle_vertex_array = elm::vertex_array::create();

		s_data.batch_circle_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(circle_vertex));
		elm::vertex_buffer_layout circle_vb_layout = {
			{ elm::shader_data_type::Float3, "a_world_position" },
			{ elm::shader_data_type::Float3, "a_local_position" },
			{ elm::shader_data_type::Float4, "a_color" },
			{ elm::shader_data_type::Float, "a_thickness" },
			{ elm::shader_data_type::Float, "a_fade" } };
		s_data.batch_circle_vertex_buffer->set_layout(&circle_vb_layout);
		s_data.batch_circle_vertex_array->add_vertex_buffer(s_data.batch_circle_vertex_buffer);

		s_data.batch_circle_vertex_buf_base = new circle_vertex[renderer_2d_data::max_sprite_verticies];

		auto circle_ib = elm::index_buffer::create(quad_indices, renderer_2d_data::max_sprite_indices);
		s_data.batch_circle_vertex_array->set_index_buffer(circle_ib);

		// -- Line vertex array --
		s_data.batch_line_vertex_array = elm::vertex_array::create();

		s_data.batch_line_vertex_buffer = elm::vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(line_vertex));
		elm::vertex_buffer_layout line_vb_layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float4, "a_color" } };
		s_data.batch_line_vertex_buffer->set_layout(&line_vb_layout);
		s_data.batch_line_vertex_array->add_vertex_buffer(s_data.batch_line_vertex_buffer);

		s_data.batch_line_vertex_buf_base = new line_vertex[renderer_2d_data::max_sprite_verticies];

		delete[] quad_indices;
	}

	extern void shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		delete[] s_data.batch_sprite_vertex_buf_base;
		s_data.batch_sprite_vertex_buf_base = nullptr;
		s_data.batch_sprite_vertex_buf_ptr = nullptr;

		delete[] s_data.batch_circle_vertex_buf_base;
		s_data.batch_circle_vertex_buf_base = nullptr;
		s_data.batch_circle_vertex_buf_ptr = nullptr;

		delete[] s_data.batch_line_vertex_buf_base;
		s_data.batch_line_vertex_buf_base = nullptr;
		s_data.batch_line_vertex_buf_ptr = nullptr;
	}

	extern void begin_scene(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.camera_buffer.view_projection = camera->get_view_projection();
		s_data.camera_uniform_buffer->bind();
		s_data.camera_uniform_buffer->set_data((const void *)&s_data.camera_buffer, sizeof s_data.camera_buffer);

		s_data.batch_sprite_count = 0u;
		s_data.batch_sprite_vertex_buf_ptr = s_data.batch_sprite_vertex_buf_base;
		s_data.batch_sprite_texture_slot_ix = 1u;

		s_data.batch_circle_count = 0u;
		s_data.batch_circle_vertex_buf_ptr = s_data.batch_circle_vertex_buf_base;

		s_data.batch_line_count = 0u;
		s_data.batch_line_vertex_buf_ptr = s_data.batch_line_vertex_buf_base;
	}

	extern void end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		flush();
	}

	static void flush_sprites(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (s_data.batch_sprite_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.batch_sprite_vertex_buf_ptr - (uint8_t *)s_data.batch_sprite_vertex_buf_base);
		s_data.batch_sprite_vertex_buffer->set_data(s_data.batch_sprite_vertex_buf_base, data_size);

		for (uint32_t i = 0; i < s_data.batch_sprite_texture_slot_ix; ++i) {
			s_data.batch_sprite_texture_slots[i]->bind(i);
		}

		s_data.sprite_shader->bind();
		s_data.batch_sprite_vertex_array->bind();
		render_command::draw_indexed(s_data.batch_sprite_vertex_array, s_data.batch_sprite_count * 6);

		s_data.batch_sprite_count = 0;
		s_data.batch_sprite_vertex_buf_ptr = s_data.batch_sprite_vertex_buf_base;
		s_data.batch_sprite_texture_slot_ix = 1;

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

	static void flush_lines(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (s_data.batch_line_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.batch_line_vertex_buf_ptr - (uint8_t *)s_data.batch_line_vertex_buf_base);
		s_data.batch_line_vertex_buffer->set_data(s_data.batch_line_vertex_buf_base, data_size);

		s_data.line_shader->bind();
		s_data.batch_line_vertex_array->bind();
		elm::render_command::set_line_thickness(s_data.line_thickness);
		render_command::draw_line(s_data.batch_line_vertex_array, s_data.batch_line_count * 2);

		s_data.batch_line_count = 0;
		s_data.batch_line_vertex_buf_ptr = s_data.batch_line_vertex_buf_base;

		++s_data.stats.draw_calls;
	}

	extern void flush(void)
	{
		flush_sprites();
		flush_circles();
		flush_lines();
	}

#pragma region Sprites

	extern void draw_sprite(
		const glm::mat4 &transform,
		const std::shared_ptr<texture_2d> &texture,
		const std::array<glm::vec2, 4> &uvs,
		const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		int texture_slot = -1;

		for (uint32_t i = 0; i < s_data.batch_sprite_texture_slot_ix; ++i) {
			if (s_data.batch_sprite_texture_slots[i]->equal(&*texture)) {
				texture_slot = (int)i;
				break;
			}
		}

		if (texture_slot == -1) {
			if (s_data.batch_sprite_texture_slot_ix >= renderer_2d_data::max_texture_slots) {
				flush_sprites();
			}

			s_data.batch_sprite_texture_slots[s_data.batch_sprite_texture_slot_ix] = texture;
			texture_slot = s_data.batch_sprite_texture_slot_ix++;
		}

		for (int i = 0; i < 4; ++i) {
			s_data.batch_sprite_vertex_buf_ptr->position = transform * s_quad_vertices[i];
			s_data.batch_sprite_vertex_buf_ptr->uv = uvs[i];
			s_data.batch_sprite_vertex_buf_ptr->color = color;
			s_data.batch_sprite_vertex_buf_ptr->texture_slot = texture_slot;
			++s_data.batch_sprite_vertex_buf_ptr;
		}

		++s_data.batch_sprite_count;
		if (s_data.batch_sprite_count >= renderer_2d_data::max_sprites) {
			flush_sprites();
		}

		++s_data.stats.quad_count;
	}

	extern void draw_sprite(
		const glm::mat4 &transform,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec2 &texture_tiling_factor,
		const glm::vec4 &color)
	{
		std::array<glm::vec2, 4> uvs = {
			s_quad_uvs[0] * texture_tiling_factor,
			s_quad_uvs[1] * texture_tiling_factor,
			s_quad_uvs[2] * texture_tiling_factor,
			s_quad_uvs[3] * texture_tiling_factor};

		draw_sprite(transform, texture, uvs, color);
	}

	void draw_sprite(const glm::mat4 &transform, const glm::vec4 &color)
	{
		draw_sprite(transform, s_data.texture_blank, s_quad_uvs, color);
	}

	extern void draw_sprite(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, s_data.texture_blank, s_quad_uvs, color);
	}

	extern void draw_sprite(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
	{
		draw_sprite(glm::vec3(position, 0.0f), size, color);
	}

	extern void draw_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, texture, s_quad_uvs, color);
	}

	extern void draw_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_sprite(glm::vec3(position, 0.0f), size, texture, color);
	}

	extern void draw_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		std::array<glm::vec2, 4> uvs = {
			s_quad_uvs[0] * texture_tiling_factor,
			s_quad_uvs[1] * texture_tiling_factor,
			s_quad_uvs[2] * texture_tiling_factor,
			s_quad_uvs[3] * texture_tiling_factor };

		draw_sprite(transform, texture, uvs, color);
	}

	extern void draw_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_sprite(glm::vec3(position, 0.0f), size, texture, texture_tiling_factor, color);
	}

	extern void draw_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, sub_texture->texture, sub_texture->uvs, color);
	}

	extern void draw_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_sprite(glm::vec3(position, 0.0f), size, sub_texture, color);
	}

	// -- Rotated sprite --

	extern void draw_rotated_sprite(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, s_data.texture_blank, s_quad_uvs, color);
	}

	extern void draw_rotated_sprite(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color)
	{
		draw_rotated_sprite(glm::vec3(position, 0.0f), size, rotation_rad, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		std::array<glm::vec2, 4> uvs = {
			s_quad_uvs[0] * texture_tiling_factor,
			s_quad_uvs[1] * texture_tiling_factor,
			s_quad_uvs[2] * texture_tiling_factor,
			s_quad_uvs[3] * texture_tiling_factor };

		draw_sprite(transform, texture, uvs, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		float texture_tiling_factor,
		const glm::vec4 &color)
	{
		draw_rotated_sprite(glm::vec3(position, 0.0f), size, rotation_rad, texture, texture_tiling_factor, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, texture, s_quad_uvs, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<texture_2d> &texture,
		const glm::vec4 &color)
	{
		draw_rotated_sprite(glm::vec3(position, 0.0f), size, rotation_rad, texture, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec3 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_rad, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		draw_sprite(transform, sub_texture->texture, sub_texture->uvs, color);
	}

	extern void draw_rotated_sprite(
		const glm::vec2 &position,
		const glm::vec2 &size,
		float rotation_rad,
		const std::shared_ptr<sub_texture_2d> &sub_texture,
		const glm::vec4 &color)
	{
		draw_rotated_sprite(glm::vec3(position, 0.0f), size, rotation_rad, sub_texture, color);
	}

#pragma endregion

#pragma region Circles

	extern void draw_circle(const glm::mat4 &transform, const glm::vec4 &color, float radius, float thickness, float fade)
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
		if (s_data.batch_circle_count >= renderer_2d_data::max_sprites) {
			flush_circles();
		}

		++s_data.stats.circle_count;
	}

#pragma endregion

#pragma region Lines

	extern float get_line_thickness(void)
	{
		return s_data.line_thickness;
	}

	extern void set_line_thickness(float thickness)
	{
		s_data.line_thickness = thickness;
	}

	extern void draw_line(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.batch_line_vertex_buf_ptr->position = p0;
		s_data.batch_line_vertex_buf_ptr->color = color;
		++s_data.batch_line_vertex_buf_ptr;

		s_data.batch_line_vertex_buf_ptr->position = p1;
		s_data.batch_line_vertex_buf_ptr->color = color;
		++s_data.batch_line_vertex_buf_ptr;

		++s_data.batch_line_count;
		if (s_data.batch_line_count >= renderer_2d_data::max_sprites) {
			flush_lines();
		}

		++s_data.stats.line_count;
	}

	extern void draw_line(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec4 &color)
	{
		draw_line(glm::vec3(p0, 0.0f), glm::vec3(p1, 0.0f), color);
	}

#pragma endregion

	extern struct statistics get_stats(void)
	{
		return s_data.stats;
	}

	extern void reset_stats(void)
	{
		memset(&s_data.stats, 0, sizeof s_data.stats);
	}
}
