#include "windows_window.hpp"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/base.hpp"
#include "elm/core/event/key_event.hpp"
#include "elm/core/event/mouse_event.hpp"
#include "elm/core/event/window_event.hpp"
#include "elm/core/log.hpp"

#include <glfw/glfw3.h>

namespace elm {

	static int s_glfw_window_count = 0;

	static void glfw_error_cb(int error, const char *description)
	{
		ELM_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
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
		ELM_PROFILE_FUNCTION();

		glfwPollEvents();
		if (!minimized) {
			this->context->swap_buffers();
		}
	}

	void windows_window::set_event_callback(const std::function<void(event &)> & cb)
	{
		this->data.event_callback = cb;
	}

	void windows_window::set_vsync(bool enabled)
	{
		ELM_PROFILE_FUNCTION();

		glfwSwapInterval(enabled ? 1 : 0);
		this->data.vsync = enabled;
	}

	void windows_window::enable_cursor(void)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void windows_window::disable_cursor(void)
	{
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void windows_window::init(const window_specification& spec)
	{
		ELM_PROFILE_FUNCTION();

		this->data.title = spec.title;
		this->data.width = spec.width;
		this->data.height = spec.height;
		this->data.vsync = spec.vsync;

		ELM_CORE_TRACE("Creating window \"{0}\" ({1}, {2})", spec.title, spec.width, spec.height);

		if (s_glfw_window_count == 0) {
			int success = glfwInit();
			glfwSetErrorCallback(glfw_error_cb);
			ELM_CORE_ASSERT(success, "Could not initialize GLFW");
		}

		++s_glfw_window_count;

		this->window = glfwCreateWindow((int)spec.width, (int)spec.height, this->data.title.c_str(), nullptr, nullptr);

		this->context = graphics_context::create(this->window);
		this->context->init();

		glfwSetWindowUserPointer(this->window, &this->data);
		set_vsync(spec.vsync);

		// --- Set glfw callbacks -------

		glfwSetWindowSizeCallback(this->window, [](GLFWwindow *window, int width, int height) {
			WindowData *data = (WindowData *)glfwGetWindowUserPointer(window);
			window_resize_event e((uint32_t)width, (uint32_t)height);
			data->width = (uint32_t)width;
			data->height = (uint32_t)height;
			data->event_callback(e);
		});

		glfwSetWindowCloseCallback(this->window, [](GLFWwindow *window) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			window_close_event e;
			data->event_callback(e);
		});

		glfwSetKeyCallback(this->window, [](GLFWwindow *window, int key_code, int scancode, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				key_pressed_event e((elm::key)key_code, 0);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				key_released_event e((elm::key)key_code);
				data->event_callback(e);
				break;
			}
			case GLFW_REPEAT: {
				key_pressed_event e((elm::key)key_code, 1);
				data->event_callback(e);
				break;
			}
			}
		});

		glfwSetCharCallback(this->window, [](GLFWwindow* window, unsigned int key_code) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			key_typed_event e((elm::key)key_code);
			data->event_callback(e);
		});

		glfwSetMouseButtonCallback(this->window, [](GLFWwindow *window, int button, int action, int mods) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS: {
				mouse_button_pressed_event e((elm::mouse)button);
				data->event_callback(e);
				break;
			}
			case GLFW_RELEASE: {
				mouse_button_released_event e((elm::mouse)button);
				data->event_callback(e);
				break;
			}
			}
		});
		
		glfwSetScrollCallback(this->window, [](GLFWwindow *window, double x_offset, double y_offset) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			mouse_scrolled_event e((float)x_offset, (float)y_offset);
			data->event_callback(e);
		});

		glfwSetCursorPosCallback(this->window, [](GLFWwindow *window, double x_pos, double y_pos) {
			WindowData *data = (WindowData*)glfwGetWindowUserPointer(window);
			mouse_moved_event e((float)x_pos, (float)y_pos);
			data->event_callback(e);
		});
	}

	void windows_window::shutdown(void)
	{
		ELM_PROFILE_FUNCTION();

		glfwDestroyWindow(this->window);
		--s_glfw_window_count;

		if (s_glfw_window_count == 0) {
			glfwTerminate();
		}
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
