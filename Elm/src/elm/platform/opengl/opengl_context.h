#pragma once

#include "elm/core/renderer/graphics_context.h"

struct GLFWwindow;

namespace elm {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow *window);

		virtual void init(void) override;
		virtual void swap_buffers(void) override;

	private:
		GLFWwindow *m_window;

	};
}
