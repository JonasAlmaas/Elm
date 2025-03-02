#include "shader_library.h"

namespace elm {

	void shader_library::add(const std::string &name, const std::shared_ptr<shader> &shader)
	{
		ELM_CORE_ASSERT(!contains(name), "Shader already exists");
		m_shaders[name] = shader;
	}

	void shader_library::add(const std::shared_ptr<shader> &shader)
	{
		add(shader->get_name(), shader);
	}

	std::shared_ptr<shader> shader_library::load(const std::string &fpath)
	{
		auto shader = shader::create(fpath);
		add(shader);
		return shader;
	}

	std::shared_ptr<shader> shader_library::load(const std::string &name, const std::string &fpath)
	{
		auto shader = shader::create(fpath);
		add(name, shader);
		return shader;
	}

	std::shared_ptr<shader> shader_library::get(const std::string &name)
	{
		ELM_CORE_ASSERT(contains(name), "Shader not found");
		return m_shaders[name];
	}
}
