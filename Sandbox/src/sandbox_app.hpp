#pragma once

#include <elm.hpp>

struct sandbox : elm::application
{
	sandbox(
		const elm::application_specification &spec,
		elm::application_command_line_args args);
};
