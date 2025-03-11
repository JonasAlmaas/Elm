#include "render_command.h"

#include "elm/platform/opengl/opengl_renderer_api.h"

namespace elm {

	std::unique_ptr<renderer_api> render_command::s_api = std::make_unique<opengl_renderer_api>();
}
