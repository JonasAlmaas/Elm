#pragma once

#include "elm/core.h"

// This ignores all warnings raised inside External headers
//#pragma warning(push, 0)
#include <spdlog/spdlog.h>
//#include <spdlog/fmt/ostr.h>
//#pragma warning(pop)

#include <memory>

namespace elm {

	class Log
	{
	public:
		static void init(void);

		static inline std::shared_ptr<spdlog::logger>& get_core_logger() { return s_core_logger; }
		static inline std::shared_ptr<spdlog::logger>& get_client_logger() { return s_client_logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_client_logger;
	};
}

#define ELM_CORE_LOG(...)               ::elm::Log::get_core_logger()->trace(__VA_ARGS__)
#define ELM_CORE_INFO(...)              ::elm::Log::get_core_logger()->info(__VA_ARGS__)
#define ELM_CORE_WARN(...)              ::elm::Log::get_core_logger()->warn(__VA_ARGS__)
#define ELM_CORE_ERROR(...)             ::elm::Log::get_core_logger()->error(__VA_ARGS__)
#define ELM_CORE_CRITICAL(...)          ::elm::Log::get_core_logger()->critical(__VA_ARGS__)

#define ELM_LOG(...)                    ::elm::Log::get_client_logger()->trace(__VA_ARGS__)
#define ELM_INFO(...)                   ::elm::Log::get_client_logger()->info(__VA_ARGS__)
#define ELM_WARN(...)                   ::elm::Log::get_client_logger()->warn(__VA_ARGS__)
#define ELM_ERROR(...)                  ::elm::Log::get_client_logger()->error(__VA_ARGS__)
#define ELM_CRITICAL(...)               ::elm::Log::get_client_logger()->critical(__VA_ARGS__)
