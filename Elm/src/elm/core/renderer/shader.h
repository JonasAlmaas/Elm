#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>
#include <string>

namespace elm {

	class shader
	{
	public:
		shader(const std::string& vertex_src, const std::string& fragment_src);
		~shader(void);

		void bind(void) const;
		void unbind(void) const;

		void upload_uniform_float4(const std::string &name, const glm::vec4 &vec) const;
		void upload_uniform_mat4(const std::string &name, const glm::mat4 &mat) const;

	private:
		uint32_t m_renderer_id;

	};
}
