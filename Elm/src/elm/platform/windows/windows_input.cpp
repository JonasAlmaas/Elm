#ifdef ELM_PLATFORM_WINDOWS

#include "windows_input.h"
#include "elm/core/application.h"

#include <GLFW/glfw3.h>

namespace elm {

	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::is_key_pressed_impl(KeyCode key_code) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->get_window()->get_native_window());
		int state = glfwGetKey(window, key_code);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::is_mouse_button_pressed_impl(MouseCode button) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->get_window()->get_native_window());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::get_mouse_pos_impl(void) const
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get()->get_window()->get_native_window());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::get_mouse_x_impl(void) const
	{
		auto [x, y] = get_mouse_pos_impl();
		return x;
	}

	float WindowsInput::get_mouse_y_impl(void) const
	{
		auto [x, y] = get_mouse_pos_impl();
		return y;
	}
}

#endif /* ELM_PLATFORM_WINDOWS */
