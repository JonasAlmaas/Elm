#include "graphics_context.hpp"

#include "renderer_api.hpp"
#include "elm/platform/opengl/opengl_context.hpp"

namespace elm {

	std::unique_ptr<graphics_context> graphics_context::create(void* window)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_unique<opengl_context>(static_cast<GLFWwindow *>(window));
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
