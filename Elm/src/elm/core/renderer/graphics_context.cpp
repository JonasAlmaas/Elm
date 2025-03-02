#include "graphics_context.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_context.h"

namespace elm {

	std::unique_ptr<graphics_context> graphics_context::create(void* window)
	{
		switch (renderer::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_unique<opengl_context>(static_cast<GLFWwindow *>(window));
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
