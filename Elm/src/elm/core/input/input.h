#pragma once

#include "elm/core/base.h"
#include "key_code.h"
#include "mouse_code.h"

namespace elm::input {

	extern bool is_key_pressed(key_code key_code);
	extern bool is_mouse_button_pressed(mouse_code button);
	extern std::pair<float, float> get_mouse_pos(void);
	extern float get_mouse_x(void);
	extern float get_mouse_y(void);
}
