#include "opengl_context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace elm {

	opengl_context::opengl_context(GLFWwindow *window)
		: window(window)
	{
		ELM_CORE_ASSERT(this->window, "Window handle is null");
	}

	void opengl_context::init()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glfwMakeContextCurrent(this->window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ELM_CORE_ASSERT(status, "Failed to initialize glad");

		ELM_CORE_INFO("OpenGL Info:");
		ELM_CORE_INFO("  Vendor: {0}", (const char *)glGetString(GL_VENDOR));
		ELM_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		ELM_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		ELM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "ELM requires at least OpenGL version 4.5");
	}

	void opengl_context::swap_buffers()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glfwSwapBuffers(this->window);
	}
}
