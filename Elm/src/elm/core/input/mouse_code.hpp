#pragma once

#include <stdint.h>

namespace elm {

	enum class mouse {
		// From glfw3.h
		BUTTON0 = 0,
		BUTTON1 = 1,
		BUTTON2 = 2,
		BUTTON3 = 3,
		BUTTON4 = 4,
		BUTTON5 = 5,
		BUTTON6 = 6,
		BUTTON7 = 7,

		BUTTON_LAST = BUTTON7,
		BUTTON_LEFT = BUTTON0,
		BUTTON_RIGHT = BUTTON1,
		BUTTON_MIDDLE = BUTTON2
	};
}
