#include "texture_atlas.h"

namespace elm {

	std::shared_ptr<texture_atlas_sprite> texture_atlas_sprite::create(
		const std::shared_ptr<texture_2d> &texture,
		uint32_t sprite_width,
		uint32_t sprite_height,
		uint32_t x,
		uint32_t y)
	{
		float texture_width = (float)texture->get_width();
		float texture_height = (float)texture->get_height();

		float fraction_x = (float)sprite_width / texture_width;
		float fraction_y = (float)sprite_height / texture_height;

		std::array<glm::vec2, 4> uvs = {
			glm::vec2(fraction_x * x, 1.0f - (fraction_y * y + fraction_y)),
			glm::vec2(fraction_x * x + fraction_x, 1.0f - (fraction_y * y + fraction_y)),
			glm::vec2(fraction_x * x + fraction_x, 1.0f - (fraction_y * y)),
			glm::vec2(fraction_x * x, 1.0f - (fraction_y * y)) };

		return std::make_shared<texture_atlas_sprite>(texture, uvs);
	}
}
