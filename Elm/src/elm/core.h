#pragma once

#ifdef ELM_PLATFORM_WINDOWS

#else
	#error Missing platform
#endif /* ELM_PLATFORM_WINDOWS */

#define BIT(n) (1<<(n))
