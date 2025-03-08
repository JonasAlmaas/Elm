#pragma once

#include "elm/core/base.h"
#include "elm/core/input/key_code.h"
#include "elm/core/input/mouse_code.h"

#ifdef ELM_PLATFORM_WINDOWS

namespace elm {

	class windows_input
	{
	public:
		bool is_key_pressed(key_code key_code) const;
		bool is_mouse_button_pressed(mouse_code button) const;
		std::pair<float, float> get_mouse_pos(void) const;
		float get_mouse_x(void) const;
		float get_mouse_y(void) const;
	};
}

#endif /* ELM_PLATFORM_WINDOWS */
