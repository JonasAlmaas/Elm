#include "sandbox_app.hpp"
#include <elm.hpp>
#include <elm/core/entypoiny.hpp>

namespace elm {

	application* application::create(application_command_line_args args)
	{
		elm::application_specification spec;
		spec.name = "Sandbox";
		return new sandbox(spec, args);
	}
}
