#include "log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace elm {

	static std::shared_ptr<spdlog::logger> s_core_logger;
	static std::shared_ptr<spdlog::logger> s_client_logger;

	extern void log::init(void)
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_core_logger = spdlog::stdout_color_mt("[ELM]");
		s_core_logger->set_level(spdlog::level::trace);

		s_client_logger = spdlog::stdout_color_mt("[APP]");
		s_client_logger->set_level(spdlog::level::trace);
	}

	extern std::shared_ptr<spdlog::logger> &log::get_core_logger()
	{
		return s_core_logger;
	}

	extern std::shared_ptr<spdlog::logger> &log::get_client_logger()
	{
		return s_client_logger;
	}
}
