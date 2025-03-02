#pragma once

#include "elm/core/renderer/graphics_context.h"

struct GLFWwindow;

namespace elm {

	class opengl_context : public graphics_context
	{
	public:
		opengl_context(GLFWwindow *window);

		virtual void init(void) override;
		virtual void swap_buffers(void) override;

	private:
		GLFWwindow *m_window;
	};
}
