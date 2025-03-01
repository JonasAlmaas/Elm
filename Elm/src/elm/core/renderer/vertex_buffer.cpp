#include "vertex_buffer.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_vertex_buffer.h"

namespace elm {

	vertex_buffer *vertex_buffer::create(const void *vertices, uint32_t size)
	{
		switch (renderer::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); break;
		case renderer_api::api::opengl:
			return new opengl_vertex_buffer(vertices, size);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
