#pragma once

#include "elm/core/base.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <memory>

namespace elm::log {

	extern void init(void);

	extern std::shared_ptr<spdlog::logger> &get_core_logger();
	extern std::shared_ptr<spdlog::logger> &get_client_logger();
}

#define ELM_CORE_TRACE(...)             ::elm::log::get_core_logger()->trace(__VA_ARGS__)
#define ELM_CORE_INFO(...)              ::elm::log::get_core_logger()->info(__VA_ARGS__)
#define ELM_CORE_WARN(...)              ::elm::log::get_core_logger()->warn(__VA_ARGS__)
#define ELM_CORE_ERROR(...)             ::elm::log::get_core_logger()->error(__VA_ARGS__)
#define ELM_CORE_CRITICAL(...)          ::elm::log::get_core_logger()->critical(__VA_ARGS__)

#define ELM_TRACE(...)                  ::elm::log::get_client_logger()->trace(__VA_ARGS__)
#define ELM_INFO(...)                   ::elm::log::get_client_logger()->info(__VA_ARGS__)
#define ELM_WARN(...)                   ::elm::log::get_client_logger()->warn(__VA_ARGS__)
#define ELM_ERROR(...)                  ::elm::log::get_client_logger()->error(__VA_ARGS__)
#define ELM_CRITICAL(...)               ::elm::log::get_client_logger()->critical(__VA_ARGS__)
