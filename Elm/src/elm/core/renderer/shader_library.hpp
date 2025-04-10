#pragma once

#include "elm/core/base.hpp"
#include "shader.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace elm {

	struct shader_library
	{
		void add(const std::string &name, const std::shared_ptr<shader> &shader);
		void add(const std::shared_ptr<shader> &shader);
		std::shared_ptr<shader> load(const std::string &fpath);
		std::shared_ptr<shader> load(const std::string &name, const std::string &fpath);

		std::shared_ptr<shader> get(const std::string &name);

		inline bool contains(const std::string &name) const { return this->shaders.find(name) != this->shaders.end(); }

		inline const std::unordered_map<std::string, std::shared_ptr<shader>> &get_shaders(void) const {return shaders;}
	private:
		std::unordered_map<std::string, std::shared_ptr<shader>> shaders;
	};
}
