#include "editor_app.hpp"
#include "editor_layer.hpp"

namespace elm {

	editor_app::editor_app(
			const struct application_specification &spec,
			struct application_command_line_args args)
		: application(spec, args)
	{
		push_layer(new editor_layer());
	}
}
