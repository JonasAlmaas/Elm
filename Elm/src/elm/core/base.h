#pragma once

// --- Config ---------------

#ifdef ELM_DEBUG
	#define ELM_ENABLE_ASSERTS 1

	#define ELM_ENABLE_PROFILING 0
	#define ELM_ENABLE_PROFILING_RENDERER 0
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
#include "elm/debug/instrumentor.h"
