#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/shader.hpp"
#include "elm/core/renderer/texture.hpp"
#include "elm/renderer/material.hpp"
#include "elm/renderer/mesh.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace elm {

	struct tag_component {
		std::string tag;

		tag_component() = default;
		tag_component(const tag_component &) = default;
	};

	struct transform_component {
		glm::mat4 transform = glm::mat4(1.0f);

		transform_component() = default;
		transform_component(const transform_component &) = default;
		transform_component(const glm::mat4 &transform)
			: transform(transform) {
		}
	};

	struct sprite_renderer_component {
		glm::vec4 color = glm::vec4(1.0f);
		std::shared_ptr<texture_2d> texture = nullptr;
		glm::vec2 tiling_factor = glm::vec2(1.0f);

		sprite_renderer_component() = default;
		sprite_renderer_component(const sprite_renderer_component &) = default;
	};

	struct circle_renderer_component {
		glm::vec4 color = glm::vec4(1.0f);
		float radius = 0.5f;
		float thickness = 1.0f;
		float fade = 0.005f;

		circle_renderer_component() = default;
		circle_renderer_component(const circle_renderer_component &) = default;
	};

	struct text_renderer_component {
		std::string text;
		std::shared_ptr<font> font;
		glm::vec4 color = glm::vec4(1.0f);
		float kerning = 0.0f;
		float line_spacing = 0.0f;

		text_renderer_component() = default;
		text_renderer_component(const text_renderer_component &) = default;
	};

	struct environment_light_component {
		std::shared_ptr<texture_cube> irradiance_map;
		std::shared_ptr<texture_cube> prefilter_map;
		std::shared_ptr<texture_2d> brdf_lut_map;

		environment_light_component() = default;
		environment_light_component(const environment_light_component&) = default;
	};

	struct directional_light_component {
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;

		directional_light_component() = default;
		directional_light_component(const directional_light_component &) = default;
	};

	struct point_light_component {
		glm::vec3 color = glm::vec3(1.0f);
		float intensity = 1.0f;

		point_light_component() = default;
		point_light_component(const point_light_component &) = default;
	};

	struct mesh_renderer_component {
		std::shared_ptr<mesh> mesh;
		std::shared_ptr<material> material; // TODO: Move to model_renderer_component

		mesh_renderer_component() = default;
		mesh_renderer_component(const mesh_renderer_component &) = default;
	};
}
