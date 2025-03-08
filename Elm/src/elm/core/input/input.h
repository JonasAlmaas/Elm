#pragma once

#include "elm/core/base.h"
#include "key_code.h"
#include "mouse_code.h"
#include <memory>

namespace elm {

	class input
	{
	public:
		inline static bool is_key_pressed(key_code key_code) { return s_instance->is_key_pressed_impl(key_code); }
		inline static bool is_mouse_button_pressed(mouse_code button) { return s_instance->is_mouse_button_pressed_impl(button); }
		inline static std::pair<float, float> get_mouse_pos(void) { return s_instance->get_mouse_pos_impl(); }
		inline static float get_mouse_x(void) { return s_instance->get_mouse_x_impl(); }
		inline static float get_mouse_y(void) { return s_instance->get_mouse_y_impl(); }

	public:
		virtual ~input(void) = default;

	protected:
		virtual bool is_key_pressed_impl(key_code key_code) const = 0;
		virtual bool is_mouse_button_pressed_impl(mouse_code button) const = 0;
		virtual std::pair<float, float> get_mouse_pos_impl(void) const = 0;
		virtual float get_mouse_x_impl(void) const = 0;
		virtual float get_mouse_y_impl(void) const = 0;

	private:
		static std::unique_ptr<input> s_instance;
	};
}
