#pragma once

#include "elm/core/renderer/graphics_context.hpp"

struct GLFWwindow;

namespace elm {

	struct opengl_context : graphics_context
	{
		opengl_context(GLFWwindow *window);

		virtual void init(void) override;
		virtual void swap_buffers(void) override;

	private:
		GLFWwindow *window;
	};
}
