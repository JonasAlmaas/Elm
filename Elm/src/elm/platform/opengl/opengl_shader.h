#pragma once

#include "elm/core/renderer/shader.h"

namespace elm {

	class opengl_shader : public shader
	{
	public:
		opengl_shader(const std::string &vertex_src, const std::string &fragment_src);
		~opengl_shader(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		void upload_uniform_float4(const std::string &name, const glm::vec4 &vec) const;
		void upload_uniform_mat4(const std::string &name, const glm::mat4 &mat) const;

	private:
		uint32_t m_renderer_id;

	};
}
