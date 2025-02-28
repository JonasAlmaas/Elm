#ifdef ELM_PLATFORM_WINDOWS

#include "windows_window.h"
#include "elm/core/base.h"
#include "elm/core/log.h"

#include <glfw/glfw3.h>

namespace elm {

	static int s_glfw_window_count = 0;

	Window *Window::create(const WindowSpecification& spec)
	{
		return new WindowsWindow(spec);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& spec)
	{
		init(spec);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::on_update(void)
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::set_event_callback(const EventCallbackFn& cb)
	{
	}

	void WindowsWindow::set_vsync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_data.vsync = enabled;
	}

	void WindowsWindow::init(const WindowSpecification& spec)
	{
		m_data.title = spec.title;
		m_data.width = spec.width;
		m_data.height = spec.height;
		m_data.vsync = spec.vsync;

		ELM_CORE_INFO("Creating window \"{0}\" ({1}, {2})", spec.title, spec.width, spec.height);

		if (s_glfw_window_count == 0) {
			int success = glfwInit();
			ELM_CORE_ASSERT(success, "Could not initialize GLFW!");
		}

		++s_glfw_window_count;

		m_window = glfwCreateWindow((int)spec.width, (int)spec.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		set_vsync(spec.vsync);
	}

	void WindowsWindow::shutdown(void)
	{
		glfwDestroyWindow(m_window);
		--s_glfw_window_count;

		if (s_glfw_window_count == 0) {
			glfwTerminate();
		}
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
