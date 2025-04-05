#pragma once

#if ELM_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define ELM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ELM##type##ERROR(msg, __VA_ARGS__); ELM_DEBUGBREAK(); } }
	#define ELM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) ELM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define ELM_INTERNAL_ASSERT_NO_MSG(type, check) ELM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", ELM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define ELM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define ELM_INTERNAL_ASSERT_GET_MACRO(...) ELM_EXPAND_MACRO( ELM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ELM_INTERNAL_ASSERT_WITH_MSG, ELM_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define ELM_ASSERT(...) ELM_EXPAND_MACRO( ELM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define ELM_CORE_ASSERT(...) ELM_EXPAND_MACRO( ELM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define ELM_ASSERT(...)
	#define ELM_CORE_ASSERT(...)
#endif
