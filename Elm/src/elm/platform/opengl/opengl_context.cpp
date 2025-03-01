#include "opengl_context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace elm {

	OpenGLContext::OpenGLContext(GLFWwindow *window)
		: m_window(window)
	{
		ELM_CORE_ASSERT(m_window, "Window handle is null");
	}

	void OpenGLContext::init(void)
	{
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ELM_CORE_ASSERT(status, "Failed to initialize glad");
	}

	void OpenGLContext::swap_buffers(void)
	{
		glfwSwapBuffers(m_window);
	}
}
