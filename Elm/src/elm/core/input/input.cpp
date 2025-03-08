#include "input.h"

#ifdef ELM_PLATFORM_WINDOWS
#include "elm/platform/windows/windows_input.h"
#endif

namespace elm {

#ifdef ELM_PLATFORM_WINDOWS
	static windows_input s_input;
#else
#error Unknown platform
#endif

	bool input::is_key_pressed(key_code key_code)
	{
		return s_input.is_key_pressed(key_code);
	}

	bool input::is_mouse_button_pressed(mouse_code button)
	{
		return s_input.is_mouse_button_pressed(button);
	}

	std::pair<float, float> input::get_mouse_pos(void)
	{
		return s_input.get_mouse_pos();
	}

	float input::get_mouse_x(void)
	{
		return s_input.get_mouse_x();
	}

	float input::get_mouse_y(void)
	{
		return s_input.get_mouse_y();
	}
}
