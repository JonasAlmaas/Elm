#include "time.h"

// Temp (used to get time)
#include <GLFW/glfw3.h>

namespace elm::time {

	extern float get_seconds(void)
	{
		return (float)glfwGetTime();
	}
}
