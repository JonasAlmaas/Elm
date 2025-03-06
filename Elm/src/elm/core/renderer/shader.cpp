#include "shader.h"

#include "renderer_api.h"
#include "elm/platform/opengl/opengl_shader.h"

namespace elm {

	std::shared_ptr<shader> shader::create(const std::string &fpath)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_shader>(fpath);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}

	std::shared_ptr<shader> shader::create(
		const std::string &name,
		const std::string &vertex_src,
		const std::string &fragment_src)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_shader>(name, vertex_src, fragment_src);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
