#include "elm/core/time.h"

#ifdef ELM_PLATFORM_WINDOWS

#include <GLFW/glfw3.h>

namespace elm::time {

	extern float get_seconds(void)
	{
		return (float)glfwGetTime();
	}
}

#endif // ELM_PLATFORM_WINDOWS
