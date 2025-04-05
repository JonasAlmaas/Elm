#pragma once

#include <elm.hpp>

struct sandbox : elm::application
{
	sandbox(
		const struct elm::application_specification &spec,
		struct elm::application_command_line_args args);
};
