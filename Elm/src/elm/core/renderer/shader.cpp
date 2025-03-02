#include "shader.h"

#include "elm/core/renderer/renderer.h"
#include "elm/platform/opengl/opengl_shader.h"

namespace elm {

	std::shared_ptr<shader> shader::create(const std::string &vertex_src, const std::string &fragment_src)
	{
		switch (renderer::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_shader>(vertex_src, fragment_src);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
