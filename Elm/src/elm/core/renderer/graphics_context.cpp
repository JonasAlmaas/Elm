#include "graphics_context.h"

#include "elm/platform/opengl/opengl_context.h"

namespace elm {

	std::unique_ptr<graphics_context> graphics_context::create(void* window)
	{
		return std::make_unique<opengl_context>(static_cast<GLFWwindow*>(window));
	}
}
