#pragma once

#include <elm.h>

class sandbox : public elm::application
{
public:
	sandbox(
		const struct elm::application_specification &spec,
		struct elm::application_command_line_args args);
};
