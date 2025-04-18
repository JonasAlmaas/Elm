#include "texture.hpp"

#include "renderer_api.hpp"
#include "elm/platform/opengl/opengl_texture.hpp"

namespace elm {

	std::shared_ptr<texture_2d> texture_2d::create(const texture_specification &spec)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture_2d>(spec);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	std::shared_ptr<texture_2d> texture_2d::create(const std::string &fpath, const texture_specification &spec)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture_2d>(fpath, spec);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	std::shared_ptr<texture_cube> texture_cube::create(const texture_specification &spec)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture_cube>(spec);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	std::shared_ptr<sub_texture_2d> sub_texture_2d::from_atlas(
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

		return std::make_shared<sub_texture_2d>(texture, uvs);
	}
}
