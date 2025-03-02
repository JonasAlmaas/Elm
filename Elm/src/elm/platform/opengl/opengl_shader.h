#pragma once

#include "elm/core/renderer/shader.h"
#include <stdint.h>
#include <unordered_map>

namespace elm {

	class opengl_shader : public shader
	{
	public:
		opengl_shader(const std::string &vertex_src, const std::string &fragment_src);
		~opengl_shader(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		void upload_uniform_int(const std::string &name, int val);

		void upload_uniform_float(const std::string &name, float val);
		void upload_uniform_float2(const std::string &name, const glm::vec2 &vec);
		void upload_uniform_float3(const std::string &name, const glm::vec3 &vec);
		void upload_uniform_float4(const std::string &name, const glm::vec4 &vec);

		void upload_uniform_mat3(const std::string &name, const glm::mat3 &mat);
		void upload_uniform_mat4(const std::string &name, const glm::mat4 &mat);

	private:
		int get_location(const std::string &name);

	private:
		uint32_t m_renderer_id;
		std::unordered_map<std::string, int> m_location_map;

	};
}
