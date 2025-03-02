#include "windows_window.h"

#ifdef ELM_PLATFORM_WINDOWS

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

	window *window::create(const window_specification& spec)
	{
		return new windows_window(spec);
	}

	windows_window::windows_window(const window_specification& spec)
	{
		init(spec);
	}

	windows_window::~windows_window()
	{
		shutdown();
	}

	void windows_window::on_update(bool minimized)
	{
		glfwPollEvents();
		if (!minimized) {
			m_context->swap_buffers();
		}
	}

	void windows_window::set_event_callback(const event_callback_fn& cb)
	{
		m_data.event_callback = cb;
	}

	void windows_window::set_vsync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_data.vsync = enabled;
	}

	void windows_window::init(const window_specification& spec)
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

		m_context = graphics_context::create(m_window);
		m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		set_vsync(spec.vsync);

		// --- Set glfw callbacks -------

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
			WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);
			window_resize_event e((uint32_t)width, (uint32_t)height);
			data->width = (uint32_t)width;
			data->height = (uint32_t)height;
			data->event_callback(e);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			window_close_event e;
			data->event_callback(e);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				key_pressed_event e(key, 0);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				key_released_event e(key);
				data->event_callback(e);
				break;
			}
			case GLFW_REPEAT: {
				key_pressed_event e(key, 1);
				data->event_callback(e);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key_code) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			key_typed_event e(key_code);
			data->event_callback(e);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				mouse_button_pressed_event e(button);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				mouse_button_released_event e(button);
				data->event_callback(e);
				break;
			}
			}
		});
		
		glfwSetScrollCallback(m_window, [](GLFWwindow *window, double x_offset, double y_offset) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			mouse_scrolled_event e((float)x_offset, (float)y_offset);
			data->event_callback(e);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x_pos, double y_pos) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			mouse_moved_event e((float)x_pos, (float)y_pos);
			data->event_callback(e);
		});
	}

	void windows_window::shutdown(void)
	{
		glfwDestroyWindow(m_window);
		--s_glfw_window_count;

		if (s_glfw_window_count == 0) {
			glfwTerminate();
		}
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
