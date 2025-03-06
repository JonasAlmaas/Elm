#include "texture.h"

#include "renderer_api.h"
#include "elm/platform/opengl/opengl_texture.h"

namespace elm {

	std::shared_ptr<texture_2d> texture_2d::create(uint32_t width, uint32_t height)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture_2d>(width, height);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	std::shared_ptr<texture_2d> texture_2d::create(const std::string &fpath)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_texture_2d>(fpath);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
