#pragma once

#include "elm/core/base.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>
#include <filesystem>

namespace elm {

	struct shader
	{
		virtual ~shader() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void reload() = 0;

		virtual const std::string &get_name() const = 0;

	public:
		static std::shared_ptr<shader> create(const std::filesystem::path &fpath);
		static std::shared_ptr<shader> create(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
	};
}
