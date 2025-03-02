#pragma once

#ifdef ELM_PLATFORM_WINDOWS

#include "elm/core/input/Input.h"

namespace elm {

	class windows_input : public input
	{
	protected:
		virtual bool is_key_pressed_impl(key_code key_code) const override;
		virtual bool is_mouse_button_pressed_impl(mouse_code button) const override;
		virtual std::pair<float, float> get_mouse_pos_impl(void) const override;
		virtual float get_mouse_x_impl(void) const override;
		virtual float get_mouse_y_impl(void) const override;
	};
}

#endif /* ELM_PLATFORM_WINDOWS */
