#pragma once

#include "elm/camera/orthographic_camera.h"
#include "elm/core/renderer/texture.h"
#include <memory>

namespace elm {

	class renderer_2d
	{
	public:
		static void init(void);
		static void shutdown(void);

		static void begin_scene(const orthographic_camera *camera);
		static void end_scene(void);

	public:
		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture> &texture, const glm::vec4 &color);
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture> &texture, const glm::vec4 &color);

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<texture> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<texture> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const glm::vec4 &color);

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture> &texture, const glm::vec4 &color);
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture> &texture, const glm::vec4 &color);

		static void draw_rotated_quad(const glm::vec3 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
		static void draw_rotated_quad(const glm::vec2 &position, const glm::vec2 &size, float rotation_rad, const std::shared_ptr<texture> &texture, float texture_tiling_factor = 1.0f, const glm::vec4 &color = glm::vec4(1.0f));
	};
}
