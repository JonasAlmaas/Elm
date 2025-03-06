#pragma once

#include "elm/core/renderer/texture.h"
#include <glm/glm.hpp>
#include <array>
#include <stdint.h>
#include <memory>

namespace elm {

	struct texture_atlas_sprite {
		const std::shared_ptr<texture_2d> texture;
		const std::array<glm::vec2, 4> uvs;

		static std::shared_ptr<texture_atlas_sprite> create(
			const std::shared_ptr<texture_2d> &texture,
			uint32_t sprite_width,
			uint32_t sprite_height,
			uint32_t x,
			uint32_t y);
	};
}
