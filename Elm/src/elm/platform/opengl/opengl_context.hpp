#pragma once

#include "elm/core/renderer/graphics_context.hpp"

struct GLFWwindow;

namespace elm {

	struct opengl_context final : graphics_context
	{
		opengl_context(GLFWwindow *window);

		virtual void init() override;
		virtual void swap_buffers() override;

	private:
		GLFWwindow *window;
	};
}
