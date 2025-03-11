#pragma once

#include <glm/glm.hpp>

namespace elm {

	struct transform_component {
		glm::mat4 transform;

		transform_component(void) = default;
		transform_component(const transform_component &) = default;
		transform_component(const glm::mat4 &transform)
			: transform(transform) {
		}
	};

	struct circle_renderer_component {
		glm::vec4 color = glm::vec4(1.0f);
		float radius = 0.5f;
		float thickness = 1.0f;
		float fade = 0.005f;

		circle_renderer_component(void) = default;
		circle_renderer_component(const circle_renderer_component &) = default;
	};
}
