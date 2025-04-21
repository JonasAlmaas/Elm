#pragma once

#include <elm.hpp>

namespace elm {

	struct editor_app : application
	{
		editor_app(
			const application_specification &spec,
			application_command_line_args args);
	};
}
