#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>
#include <string>

namespace elm {

	class shader
	{
	public:
		virtual ~shader(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual const std::string &get_name(void) const = 0;

		virtual void set_int(const std::string &name, int val) = 0;
		virtual void set_int_array(const std::string &name, int *vals, uint32_t count) = 0;

		virtual void set_float(const std::string &name, float val) = 0;
		virtual void set_float2(const std::string &name, const glm::vec2 &vec) = 0;
		virtual void set_float3(const std::string &name, const glm::vec3 &vec) = 0;
		virtual void set_float4(const std::string &name, const glm::vec4 &vec) = 0;

		virtual void set_mat3(const std::string &name, const glm::mat3 &mat) = 0;
		virtual void set_mat4(const std::string &name, const glm::mat4 &mat) = 0;

	public:
		static std::shared_ptr<shader> create(const std::string &fpath);
		static std::shared_ptr<shader> create(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
	};
}
