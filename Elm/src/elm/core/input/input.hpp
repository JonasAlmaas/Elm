#pragma once

#include "elm/core/base.hpp"
#include "key_code.hpp"
#include "mouse_code.hpp"

namespace elm::input {

	extern bool is_key_pressed(key_code key_code);
	extern bool is_mouse_button_pressed(mouse_code button);
	extern std::pair<float, float> get_mouse_pos(void);
	extern float get_mouse_x(void);
	extern float get_mouse_y(void);
}
