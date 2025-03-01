#include "index_buffer.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_index_buffer.h"

namespace elm {

	index_buffer *index_buffer::create(const uint32_t *indices, uint32_t count)
	{
		switch (renderer::get_api()) {
		case renderer_api::none: ELM_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); break;
		case renderer_api::opengl:
			return new opengl_index_buffer(indices, count);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
