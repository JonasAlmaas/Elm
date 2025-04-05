#include "renderer_2d.hpp"
#include "elm/core/renderer/render_command.hpp"
#include "elm/core/renderer/shader.hpp"
#include "elm/core/renderer/texture.hpp"
#include "elm/core/renderer/uniform_buffer.hpp"
#include "elm/core/renderer/vertex_array.hpp"
#include "elm/renderer/msdf_data.hpp"
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

	struct text_vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;
		int texture_slot;
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
		std::shared_ptr<shader> text_shader;

		std::shared_ptr<texture_2d> texture_blank; // Texture slot 0

		// Sprite
		std::shared_ptr<vertex_array> batch_sprite_vertex_array;
		std::shared_ptr<vertex_buffer> batch_sprite_vertex_buffer;
		uint32_t batch_sprite_count = 0u;
		sprite_vertex *batch_sprite_vertex_buf_base = nullptr;
		sprite_vertex *batch_sprite_vertex_buf_ptr = nullptr;
		std::array<std::shared_ptr<texture_2d>, max_texture_slots> batch_sprite_texture_slots;
		uint32_t batch_sprite_texture_slot_ix = 1u; // 0 = blank texture

		// Circle
		std::shared_ptr<vertex_array> batch_circle_vertex_array;
		std::shared_ptr<vertex_buffer> batch_circle_vertex_buffer;
		uint32_t batch_circle_count = 0u;
		circle_vertex *batch_circle_vertex_buf_base = nullptr;
		circle_vertex *batch_circle_vertex_buf_ptr = nullptr;

		// Line
		float line_thickness = 2.0f;
		std::shared_ptr<vertex_array> batch_line_vertex_array;
		std::shared_ptr<vertex_buffer> batch_line_vertex_buffer;
		uint32_t batch_line_count = 0u;
		line_vertex *batch_line_vertex_buf_base = nullptr;
		line_vertex *batch_line_vertex_buf_ptr = nullptr;

		// Text
		std::shared_ptr<vertex_array> batch_text_vertex_array;
		std::shared_ptr<vertex_buffer> batch_text_vertex_buffer;
		uint32_t batch_text_glyph_count = 0u;
		text_vertex *batch_text_vertex_buf_base = nullptr;
		text_vertex *batch_text_vertex_buf_ptr = nullptr;
		std::array<std::shared_ptr<texture_2d>, max_texture_slots> batch_text_texture_slots;
		uint32_t batch_text_texture_slot_ix = 0u;

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
		s_data.sprite_shader = shader::create("content/shaders/renderer_2d_sprite.glsl");
		s_data.circle_shader = shader::create("content/shaders/renderer_2d_circle.glsl");
		s_data.line_shader = shader::create("content/shaders/renderer_2d_line.glsl");
		s_data.text_shader = shader::create("content/shaders/renderer_2d_text.glsl");

		// Create blank white texture
		s_data.texture_blank = texture_2d::create({ .width = 1, .height = 1 });
		uint32_t t = 0xFFFFFFFFu;
		s_data.texture_blank->set_data(&t, sizeof t);
		s_data.batch_sprite_texture_slots[0] = s_data.texture_blank;

		// -- Quad vertex array --
		s_data.batch_sprite_vertex_array = vertex_array::create();

		s_data.batch_sprite_vertex_buffer = vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(sprite_vertex));
		vertex_buffer_layout quad_vb_layout = {
			{ shader_data_type::FLOAT3, "a_position" },
			{ shader_data_type::FLOAT2, "a_uv" },
			{ shader_data_type::FLOAT4, "a_color" },
			{ shader_data_type::INT, "a_texture_slot" } };
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
		auto quad_ib = index_buffer::create(quad_indices, renderer_2d_data::max_sprite_indices);
		s_data.batch_sprite_vertex_array->set_index_buffer(quad_ib);

		// -- Circle vertex array --
		s_data.batch_circle_vertex_array = vertex_array::create();

		s_data.batch_circle_vertex_buffer = vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(circle_vertex));
		vertex_buffer_layout circle_vb_layout = {
			{ shader_data_type::FLOAT3, "a_world_position" },
			{ shader_data_type::FLOAT3, "a_local_position" },
			{ shader_data_type::FLOAT4, "a_color" },
			{ shader_data_type::FLOAT, "a_thickness" },
			{ shader_data_type::FLOAT, "a_fade" } };
		s_data.batch_circle_vertex_buffer->set_layout(&circle_vb_layout);
		s_data.batch_circle_vertex_array->add_vertex_buffer(s_data.batch_circle_vertex_buffer);

		s_data.batch_circle_vertex_buf_base = new circle_vertex[renderer_2d_data::max_sprite_verticies];

		auto circle_ib = index_buffer::create(quad_indices, renderer_2d_data::max_sprite_indices);
		s_data.batch_circle_vertex_array->set_index_buffer(circle_ib);

		// -- Line vertex array --
		s_data.batch_line_vertex_array = vertex_array::create();

		s_data.batch_line_vertex_buffer = vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(line_vertex));
		vertex_buffer_layout line_vb_layout = {
			{ shader_data_type::FLOAT3, "a_position" },
			{ shader_data_type::FLOAT4, "a_color" } };
		s_data.batch_line_vertex_buffer->set_layout(&line_vb_layout);
		s_data.batch_line_vertex_array->add_vertex_buffer(s_data.batch_line_vertex_buffer);

		s_data.batch_line_vertex_buf_base = new line_vertex[renderer_2d_data::max_sprite_verticies];

		// -- Text vertex array --
		s_data.batch_text_vertex_array = vertex_array::create();

		s_data.batch_text_vertex_buffer = vertex_buffer::create(renderer_2d_data::max_sprite_verticies * sizeof(text_vertex));
		vertex_buffer_layout text_vb_layout = {
			{ shader_data_type::FLOAT3, "a_position" },
			{ shader_data_type::FLOAT2, "a_uv" },
			{ shader_data_type::FLOAT4, "a_color" },
			{ shader_data_type::INT, "a_texture_slot" } };
		s_data.batch_text_vertex_buffer->set_layout(&text_vb_layout);
		s_data.batch_text_vertex_array->add_vertex_buffer(s_data.batch_text_vertex_buffer);

		auto text_ib = index_buffer::create(quad_indices, renderer_2d_data::max_sprite_indices);
		s_data.batch_text_vertex_array->set_index_buffer(text_ib);

		s_data.batch_text_vertex_buf_base = new text_vertex[renderer_2d_data::max_sprite_verticies];

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

		delete[] s_data.batch_text_vertex_buf_base;
		s_data.batch_text_vertex_buf_base = nullptr;
		s_data.batch_text_vertex_buf_ptr = nullptr;
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

		s_data.batch_text_glyph_count = 0u;
		s_data.batch_text_vertex_buf_ptr = s_data.batch_text_vertex_buf_base;
		s_data.batch_text_texture_slot_ix = 0u;
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
		render_command::set_line_thickness(s_data.line_thickness);
		render_command::draw_line(s_data.batch_line_vertex_array, s_data.batch_line_count * 2);

		s_data.batch_line_count = 0;
		s_data.batch_line_vertex_buf_ptr = s_data.batch_line_vertex_buf_base;

		++s_data.stats.draw_calls;
	}

	static void flush_text(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (s_data.batch_text_glyph_count == 0) {
			return;
		}

		uint32_t data_size = (uint32_t)((uint8_t *)s_data.batch_text_vertex_buf_ptr - (uint8_t *)s_data.batch_text_vertex_buf_base);
		s_data.batch_text_vertex_buffer->set_data(s_data.batch_text_vertex_buf_base, data_size);

		for (uint32_t i = 0; i < s_data.batch_text_texture_slot_ix; ++i) {
			s_data.batch_text_texture_slots[i]->bind(i);
		}

		s_data.text_shader->bind();
		s_data.batch_text_vertex_array->bind();
		render_command::draw_indexed(s_data.batch_text_vertex_array, s_data.batch_text_glyph_count * 6);

		s_data.batch_text_glyph_count  = 0;
		s_data.batch_text_vertex_buf_ptr = s_data.batch_text_vertex_buf_base;
		s_data.batch_text_texture_slot_ix = 1;

		++s_data.stats.draw_calls;
	}

	extern void flush(void)
	{
		flush_sprites();
		flush_circles();
		flush_lines();
		flush_text();
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

#pragma region Text

	extern void draw_text(
		const std::string &text,
		const std::shared_ptr<font> &font,
		const glm::mat4 &transform,
		const struct text_render_params &params)
	{
		const auto &font_geometry = font->get_msdf_data()->font_geometry;
		const auto &metrics = font_geometry.getMetrics();
		auto &atlas_texture = font->get_atlas_texture();

		int texture_slot = -1;

		for (uint32_t i = 0; i < s_data.batch_text_texture_slot_ix; ++i) {
			if (s_data.batch_text_texture_slots[i]->equal(&*atlas_texture)) {
				texture_slot = (int)i;
				break;
			}
		}

		if (texture_slot == -1) {
			if (s_data.batch_text_texture_slot_ix >= renderer_2d_data::max_texture_slots) {
				flush_text();
			}

			s_data.batch_text_texture_slots[s_data.batch_text_texture_slot_ix] = atlas_texture;
			texture_slot = s_data.batch_text_texture_slot_ix++;
		}

		double x = 0.0, y = 0.0;
		double fs_scale = 1.0 / (metrics.ascenderY - metrics.descenderY);

		const float space_glyph_advance = (float)font_geometry.getGlyph(' ')->getAdvance();

		for (size_t i = 0; i < text.size(); ++i) {
			char character = text[i];
			if (character == '\r') {
				continue;
			}

			if (character == '\n') {
				x = 0.0;
				y -= fs_scale * metrics.lineHeight + params.line_spacing;
				continue;
			}

			if (character == ' ') {
				float advance = space_glyph_advance;
				if (i < text.size() - 1) {
					double d_advance;
					char next_character = text[i + 1];
					font_geometry.getAdvance(d_advance, character, next_character);
					advance = (float)d_advance;
				}
				x += fs_scale * advance + params.kerning;
				continue;
			}

			if (character == '\t') {
				// Is this what we want?
				x += 4.0f * (fs_scale * space_glyph_advance + params.kerning);
			}

			auto glyph = font_geometry.getGlyph(character);
			if (!glyph) {
				glyph = font_geometry.getGlyph('?');
				if (!glyph) {
					ELM_CORE_ASSERT(false, "Unable to load missing glpyh glpyh");
					return;
				}
			}

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 uv_min((float)al, (float)ab);
			glm::vec2 uv_max((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quad_min((float)pl, (float)pb);
			glm::vec2 quad_max((float)pr, (float)pt);

			quad_min *= fs_scale, quad_max *= fs_scale;
			quad_min += glm::vec2(x, y);
			quad_max += glm::vec2(x, y);

			float texel_width = 1.0f / atlas_texture->get_width();
			float texel_height = 1.0f / atlas_texture->get_height();
			uv_min *= glm::vec2(texel_width, texel_height);
			uv_max *= glm::vec2(texel_width, texel_height);

			// Render
			s_data.batch_text_vertex_buf_ptr->position = transform * glm::vec4(quad_min, 0.0f, 1.0f);;
			s_data.batch_text_vertex_buf_ptr->uv = uv_min;
			s_data.batch_text_vertex_buf_ptr->color = params.color;
			s_data.batch_text_vertex_buf_ptr->texture_slot = texture_slot;
			++s_data.batch_text_vertex_buf_ptr;

			s_data.batch_text_vertex_buf_ptr->position = transform * glm::vec4(quad_max.x, quad_min.y, 0.0f, 1.0f);;
			s_data.batch_text_vertex_buf_ptr->uv = { uv_max.x, uv_min.y };
			s_data.batch_text_vertex_buf_ptr->color = params.color;
			s_data.batch_text_vertex_buf_ptr->texture_slot = texture_slot;
			++s_data.batch_text_vertex_buf_ptr;

			s_data.batch_text_vertex_buf_ptr->position = transform * glm::vec4(quad_max, 0.0f, 1.0f);;
			s_data.batch_text_vertex_buf_ptr->uv = uv_max;
			s_data.batch_text_vertex_buf_ptr->color = params.color;
			s_data.batch_text_vertex_buf_ptr->texture_slot = texture_slot;
			++s_data.batch_text_vertex_buf_ptr;

			s_data.batch_text_vertex_buf_ptr->position = transform * glm::vec4(quad_min.x, quad_max.y, 0.0f, 1.0f);;
			s_data.batch_text_vertex_buf_ptr->uv = { uv_min.x, uv_max.y };
			s_data.batch_text_vertex_buf_ptr->color = params.color;
			s_data.batch_text_vertex_buf_ptr->texture_slot = texture_slot;
			++s_data.batch_text_vertex_buf_ptr;

			++s_data.batch_text_glyph_count;

			if (i < text.size() - 1) {
				double advance = glyph->getAdvance();
				char next_character = text[i + 1];
				font_geometry.getAdvance(advance, character, next_character);

				x += fs_scale * advance + params.kerning;
			}
		}

		if (s_data.batch_text_glyph_count >= renderer_2d_data::max_sprites) {
			flush_text();
		}
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
