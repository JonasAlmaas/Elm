#pragma once

#include <elm.h>

struct sandbox : elm::application
{
	sandbox(const struct elm::application_specification &spec,
		struct elm::application_command_line_args args);
};
