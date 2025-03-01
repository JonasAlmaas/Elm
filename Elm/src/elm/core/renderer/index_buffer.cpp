#include "index_buffer.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_index_buffer.h"

namespace elm {

	IndexBuffer *IndexBuffer::create(const uint32_t *indices, uint32_t count)
	{
		switch (Renderer::get_api()) {
		case RendererAPI::None: ELM_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); break;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		}

		ELM_CORE_ASSERT(false, "Unknown rrenderer API");
		return nullptr;
	}
}
