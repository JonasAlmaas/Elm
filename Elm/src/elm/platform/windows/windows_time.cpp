#include "elm/core/time.hpp"

#ifdef ELM_PLATFORM_WINDOWS

#include <GLFW/glfw3.h>

namespace elm::time {

	extern float get_seconds()
	{
		return (float)glfwGetTime();
	}
}

#endif // ELM_PLATFORM_WINDOWS
