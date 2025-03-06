#pragma once

#include "elm/camera/orthographic_camera.h"
#include "elm/core/renderer/texture.h"
#include "texture_atlas.h"
#include <memory>

namespace elm {

	class renderer_2d
	{
	public:
		static void init(void);
		static void shutdown(void);

		static void begin_scene(const orthographic_camera *camera);
		static void end_scene(void);

		static void flush(void);

	private:
		static void draw_quad_transform(
			const glm::mat4 &transform,
			const std::shared_ptr<texture_2d> &texture,
			const std::array<glm::vec2, 4> *uvs,
			const glm::vec2 &texture_tiling_factor,
			const glm::vec4 &color);

		static void draw_quad_super(
			const glm::vec3 &position,
			const glm::vec2 &size,
			const std::shared_ptr<texture_2d> &texture,
			const std::array<glm::vec2, 4> *uvs,
			const glm::vec2 &texture_tiling_factor,
			const glm::vec4 &color);

		static void draw_quad_super_rotated(
			const glm::vec3 &position,
			const glm::vec2 &size,
			float rotation_rad,
			const std::shared_ptr<texture_2d> &texture,
			const std::array<glm::vec2, 4> *uvs,
			const glm::vec2 &texture_tiling_factor,
			const glm::vec4 &color);

	public:
		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture_atlas_sprite> &texture_sprite, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture_atlas_sprite> &texture_sprite, const glm::vec4 &color = glm::vec4(1.0f));

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, const glm::vec4 &color);

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_2d> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_atlas_sprite> &texture_sprite, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture_atlas_sprite> &texture_sprite, const glm::vec4 &color = glm::vec4(1.0f));

	public:
		struct statistics {
			uint32_t draw_calls = 0u;
			uint32_t quad_count = 0u;

			inline uint32_t get_vertex_count(void) const { return quad_count * 4u; }
			inline uint32_t get_index_count(void) const { return quad_count * 6u; }
			uint32_t get_memory_usage(void) const;
		};
		static statistics get_stats(void);
		static void reset_stats(void);
	};
}
