#pragma once

#include "elm/core/base.hpp"
#include "key_code.hpp"
#include "mouse_code.hpp"

namespace elm::input {

	extern bool key_pressed(key key_code);
	template<int... KEY_CODE> inline bool any_key_pressed(void)
	{
		return (key_pressed(KEY_CODE) || ...);
	}
	template<int... KEY_CODE> inline bool all_keys_pressed(void)
	{
		return (key_pressed(KEY_CODE) && ...);
	}

	extern bool mouse_button_pressed(mouse button);

	extern std::pair<float, float> get_mouse_pos(void);
	inline float get_mouse_x(void)
	{
		auto [x, y] = get_mouse_pos();
		return x;
	}
	inline float get_mouse_y(void)
	{
		auto [x, y] = get_mouse_pos();
		return y;
	}
}
