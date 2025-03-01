#include "graphics_context.h"

#include "elm/platform/opengl/opengl_context.h"

namespace elm {

	std::unique_ptr<GraphicsContext> GraphicsContext::create(void* window)
	{
		return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}
}
