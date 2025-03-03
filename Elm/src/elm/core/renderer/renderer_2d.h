#pragma once

#include "elm/camera/orthographic_camera.h"
#include <memory>

namespace elm {

	class renderer_2d
	{
	public:
		static void init(void);
		static void shutdown(void);

		static void begin_scene(const orthographic_camera *camera);
		static void end_scene(void);

		static void draw_quad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
		static void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
	};
}
