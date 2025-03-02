#pragma once

#include "elm/core/base.h"
#include "shader.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace elm {

	class shader_library
	{
	public:
		void add(const std::string &name, const std::shared_ptr<shader> &shader);
		void add(const std::shared_ptr<shader> &shader);
		std::shared_ptr<shader> load(const std::string &fpath);
		std::shared_ptr<shader> load(const std::string &name, const std::string &fpath);

		std::shared_ptr<shader> get(const std::string &name);

		inline bool contains(const std::string &name) const { return m_shaders.find(name) != m_shaders.end(); }

	private:
		std::unordered_map<std::string, std::shared_ptr<shader>> m_shaders;
	};
}
