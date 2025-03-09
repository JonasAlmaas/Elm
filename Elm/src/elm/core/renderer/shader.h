#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>
#include <filesystem>

namespace elm {

	class shader
	{
	public:
		virtual ~shader(void) = default;

		virtual void bind(void) = 0;
		virtual void unbind(void) = 0;

		virtual void reload(void) = 0;

		virtual const std::string &get_name(void) const = 0;

	public:
		static std::shared_ptr<shader> create(const std::filesystem::path &fpath);
		static std::shared_ptr<shader> create(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
	};
}
