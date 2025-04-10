#pragma once

#include <elm.hpp>

namespace elm {

	struct editor_app : application
	{
		editor_app(
			const struct application_specification &spec,
			struct application_command_line_args args);
	};
}
