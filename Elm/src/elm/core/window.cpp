#include "window.h"

#ifdef ELM_PLATFORM_WINDOWS
#include "elm/platform/windows/windows_window.h"
#endif

namespace elm {

	std::unique_ptr<window> window::create(const window_specification &spec)
	{
#ifdef ELM_PLATFORM_WINDOWS
		return std::make_unique<windows_window>(spec);
#else
		ELM_CORE_ASSERT(false, "Unknown platform")
		return nullptr;
#endif
	}
}
