#pragma once

// Platform detection
#ifdef _WIN32 /* Windows x64/x86 */
	#ifdef _WIN64 /* Windows x64  */
		#define ELM_PLATFORM_WINDOWS
	#else /* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define ELM_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define ELM_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define ELM_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define ELM_PLATFORM_LINUX
	#error "Linux is not supported!"
#else /* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

// --- Config ---------------

#ifdef ELM_DEBUG
	#define ELM_ENABLE_ASSERTS 1
#else
	#define ELM_ENABLE_ASSERTS 0
#endif

// --------------------------

#ifdef ELM_DEBUG
	#ifdef ELM_PLATFORM_WINDOWS
		#define ELM_DEBUGBREAK() __debugbreak()
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define ME_DEBUGBREAK()
#endif

#ifdef ELM_PLATFORM_WINDOWS

#else
	#error Missing platform
#endif

// --- Macros ---------------

#define ELM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define ELM_EXPAND_MACRO(x) x
#define ELM_STRINGIFY_MACRO(x) #x

// --------------------------

#include "elm/core/log.h"
#include "elm/core/assert.h"
