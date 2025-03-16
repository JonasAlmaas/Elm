#pragma once

#include "elm/core/renderer/camera.h"
#include "elm/core/renderer/texture.h"
#include "elm/renderer/font.h"
#include <memory>

namespace elm::renderer_2d {

	struct statistics {
		uint32_t draw_calls = 0u;
		uint32_t quad_count = 0u;
		uint32_t circle_count = 0u;
		uint32_t line_count = 0u;

		inline uint32_t get_vertex_count(void) const { return quad_count * 4u + circle_count * 4u + line_count * 2u; }
		inline uint32_t get_index_count(void) const { return quad_count * 6u + circle_count * 6u; }
	};

	extern void init(void);
	extern void shutdown(void);

	extern void begin_scene(const camera *camera);
	extern void end_scene(void);

	extern void flush(void);

	// -- Draw sprite --
	extern void draw_sprite(const glm::mat4 &transform, const std::shared_ptr<texture_2d> &texture, const std::array<glm::vec2, 4> &uvs, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_sprite(const glm::mat4 &transform, const std::shared_ptr<texture_2d> &texture, const glm::vec2 &texture_tiling_factor = glm::vec2(1.0f), const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_sprite(const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_sprite(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_sprite(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_sprite(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);
	extern void draw_sprite(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);

	extern void draw_sprite(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_sprite(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_sprite(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_sprite(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_sprite(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad = 0.0f, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_sprite(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad = 0.0f, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_sprite(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_sprite(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_sprite(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_sprite(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_sprite(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_sprite(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));

	// -- Draw circle --
	extern void draw_circle(const glm::mat4 &transform, const glm::vec4 &color, float radius = 0.5f, float thickness = 1.0f, float fade = 0.005f);

	// -- Draw line --
	extern float get_line_thickness(void);
	extern void set_line_thickness(float thickness);

	extern void draw_line(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_line(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec4 &color = glm::vec4(1.0f));

	// -- Draw text --
	extern void draw_text(const std::string &text, const std::shared_ptr<font> &font, const glm::mat4 &transform, const glm::vec4 &color = glm::vec4(1.0f));

	// -- Statistics --
	extern struct statistics get_stats(void);
	extern void reset_stats(void);
}
