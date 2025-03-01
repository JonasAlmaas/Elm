#include "vertex_array.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_vertex_array.h"

namespace elm {

	vertex_array *vertex_array::create(void)
	{
		switch (renderer::get_api()) {
		case renderer_api::none: ELM_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); break;
		case renderer_api::opengl:
			return new opengl_vertex_array();
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
