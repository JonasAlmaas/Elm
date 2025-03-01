#include "renderer.h"
#include "render_command.h"

namespace elm {

	void renderer::begin_scene(void)
	{

	}

	void renderer::end_scene(void)
	{

	}

	void renderer::submit(const std::shared_ptr<vertex_array> &vertex_array)
	{
		render_command::draw_indexed(vertex_array);
	}
}
