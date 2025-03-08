#include "elm/core/input/input.h"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/application.h"
#include <GLFW/glfw3.h>

namespace elm {

	bool input::is_key_pressed(key_code key_code)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetKey(window, key_code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::is_mouse_button_pressed(mouse_code button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> input::get_mouse_pos(void)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float input::get_mouse_x(void)
	{
		auto [x, y] = get_mouse_pos();
		return x;
	}

	float input::get_mouse_y(void)
	{
		auto [x, y] = get_mouse_pos();
		return y;
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
