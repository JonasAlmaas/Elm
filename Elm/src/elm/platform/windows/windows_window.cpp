#ifdef ELM_PLATFORM_WINDOWS

#include "windows_window.h"
#include "elm/core/base.h"
#include "elm/core/event/key_event.h"
#include "elm/core/event/mouse_event.h"
#include "elm/core/event/window_event.h"
#include "elm/core/log.h"

#include <glfw/glfw3.h>

namespace elm {

	static int s_glfw_window_count = 0;

	static void glfw_error_cb(int error, const char *description)
	{
		ELM_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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
		m_context->swap_buffers();
	}

	void WindowsWindow::set_event_callback(const EventCallbackFn& cb)
	{
		m_data.event_callback = cb;
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


		ELM_CORE_TRACE("Creating window \"{0}\" ({1}, {2})", spec.title, spec.width, spec.height);

		if (s_glfw_window_count == 0) {
			int success = glfwInit();
			glfwSetErrorCallback(glfw_error_cb);
			ELM_CORE_ASSERT(success, "Could not initialize GLFW");
		}

		++s_glfw_window_count;

		m_window = glfwCreateWindow((int)spec.width, (int)spec.height, m_data.title.c_str(), nullptr, nullptr);

		m_context = GraphicsContext::create(m_window);
		m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		set_vsync(spec.vsync);

		// --- Set glfw callbacks -------

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
			WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);
			WindowResizeEvent e((uint32_t)width, (uint32_t)height);
			data->width = (uint32_t)width;
			data->height = (uint32_t)height;
			data->event_callback(e);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data->event_callback(e);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent e(key, 0);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent e(key);
				data->event_callback(e);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent e(key, 1);
				data->event_callback(e);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key_code) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(key_code);
			data->event_callback(e);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent e(button);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent e(button);
				data->event_callback(e);
				break;
			}
			}
		});
		
		glfwSetScrollCallback(m_window, [](GLFWwindow *window, double x_offset, double y_offset) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((float)x_offset, (float)y_offset);
			data->event_callback(e);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x_pos, double y_pos) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e((float)x_pos, (float)y_pos);
			data->event_callback(e);
		});
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
