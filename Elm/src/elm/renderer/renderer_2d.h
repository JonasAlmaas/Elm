#pragma once

#include "elm/camera/orthographic_camera.h"
#include "elm/core/renderer/texture.h"
#include <memory>

namespace elm::renderer_2d {

	struct statistics {
		uint32_t draw_calls = 0u;
		uint32_t quad_count = 0u;

		inline uint32_t get_vertex_count(void) const { return quad_count * 4u; }
		inline uint32_t get_index_count(void) const { return quad_count * 6u; }
		uint32_t get_memory_usage(void) const;
	};

	extern void init(void);
	extern void shutdown(void);

	extern void begin_scene(const orthographic_camera *camera);
	extern void end_scene(void);

	extern void flush(void);

	// -- Draw quad --
	extern void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
	extern void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

	extern void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);
	extern void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);

	extern void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);
	extern void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);

	extern void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);
	extern void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);

	extern void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

	extern void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));
	extern void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<sub_texture_2d> &sub_texture, const glm::vec4 &color = glm::vec4(1.0f));

	// -- Statistics --
	extern struct statistics get_stats(void);
	extern void reset_stats(void);
}
