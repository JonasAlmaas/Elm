#pragma once

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/input/Input.h"

namespace elm {

	class WindowsInput : public Input
	{
	protected:
		virtual bool is_key_pressed_impl(KeyCode key_code) const override;
		virtual bool is_mouse_button_pressed_impl(MouseCode button) const override;
		virtual std::pair<float, float> get_mouse_pos_impl(void) const override;
		virtual float get_mouse_x_impl(void) const override;
		virtual float get_mouse_y_impl(void) const override;

	};
}

#endif /* ELM_PLATFORM_WINDOWS */
