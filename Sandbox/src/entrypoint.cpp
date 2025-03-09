#include <elm.h>
#include <elm/core/entypoiny.h>

#include "sandbox_app.h"

namespace elm {

	application* application::create(struct application_command_line_args args)
	{
		return new sandbox(
			{
				.name = "Sandbox"
			},
			args);
	}
}
