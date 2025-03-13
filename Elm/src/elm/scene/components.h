#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include "elm/renderer/mesh.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

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

	struct mesh_renderer_component {
		std::shared_ptr<mesh> mesh;
		std::shared_ptr<shader> shader;
		std::vector<std::shared_ptr<texture_2d>> textures;
	};

	struct directional_light_component {
		glm::vec3 direction;
		glm::vec3 color;
		glm::vec3 ambient_color;
		float intensity;
		float ambient_intensity;
	};
}
