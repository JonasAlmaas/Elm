#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>

// quick_and_dirty_mesh_renderer
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/vertex_array.h"
#include "elm/core/renderer/texture.h"
#include <memory>

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

	struct quick_and_dirty_mesh_renderer {
		std::shared_ptr<shader> shader;
		std::shared_ptr<vertex_array> vertex_array;
		std::shared_ptr<texture_2d> texture;
	};
}
