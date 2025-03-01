#include "vertex_buffer.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_vertex_buffer.h"

namespace elm {

	VertexBuffer *VertexBuffer::create(const void *vertices, uint32_t size)
	{
		switch (Renderer::get_api()) {
		case RendererAPI::None: ELM_CORE_ASSERT(false, "RendererAPI::None is not currently supported"); break;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		}

		ELM_CORE_ASSERT(false, "Unknown rrenderer API");
		return nullptr;
	}
}
