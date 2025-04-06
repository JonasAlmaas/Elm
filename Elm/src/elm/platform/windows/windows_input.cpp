#include "elm/core/input/input.hpp"

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/application.hpp"
#include <GLFW/glfw3.h>

namespace elm::input {

	extern bool key_pressed(key_code key_code)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetKey(window, key_code);
		return state == GLFW_PRESS;
	}

	extern bool mouse_button_pressed(mouse_code button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	extern std::pair<float, float> get_mouse_pos(void)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(application::get()->get_window()->get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
