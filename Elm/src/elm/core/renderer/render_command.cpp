#include "render_command.hpp"

#include "elm/platform/opengl/opengl_renderer_api.hpp"

namespace elm {

	std::unique_ptr<renderer_api> render_command::s_api = std::make_unique<opengl_renderer_api>();
}
