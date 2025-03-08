#pragma once

#include "elm/core/base.h"
#include "key_code.h"
#include "mouse_code.h"

namespace elm {

	class input
	{
	public:
		static bool is_key_pressed(key_code key_code);
		static bool is_mouse_button_pressed(mouse_code button);
		static std::pair<float, float> get_mouse_pos(void);
		static float get_mouse_x(void);
		static float get_mouse_y(void);
	};
}
