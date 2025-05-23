#pragma once

#include "elm/core/renderer/camera.hpp"
#include "elm/core/renderer/texture.hpp"
#include "elm/renderer/font.hpp"
#include <memory>

namespace elm::renderer_2d {

	struct statistics {
		uint32_t draw_calls = 0u;
	};

	struct text_render_params {
		glm::vec4 color = glm::vec4(1.0f);
		float kerning = 0.0f;
		float line_spacing = 0.0f;
	};

	extern void init();
	extern void shutdown();

	extern void begin_scene(const camera *camera);
	extern void end_scene();

	extern void flush();

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
	extern float get_line_thickness();
	extern void set_line_thickness(float thickness);

	extern void draw_line(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_line(const glm::vec2 &p0, const glm::vec2 &p1, const glm::vec4 &color = glm::vec4(1.0f));

	// -- Draw text --
	extern void draw_text(const std::string &text, const std::shared_ptr<font> &font, const glm::mat4 &transform, const text_render_params &params);

	// -- Statistics --
	extern statistics get_stats();
	extern void reset_stats();
}
