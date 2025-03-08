#include "windows_input.h"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/application.h"
#include <GLFW/glfw3.h>

namespace elm {

	bool windows_input::is_key_pressed(key_code key_code) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetKey(window, key_code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool windows_input::is_mouse_button_pressed(mouse_code button) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> windows_input::get_mouse_pos(void) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float windows_input::get_mouse_x(void) const
	{
		auto [x, y] = get_mouse_pos();
		return x;
	}

	float windows_input::get_mouse_y(void) const
	{
		auto [x, y] = get_mouse_pos();
		return y;
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
