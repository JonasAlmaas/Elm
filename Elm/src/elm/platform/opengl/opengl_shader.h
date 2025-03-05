#pragma once

#include "elm/core/renderer/shader.h"
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace elm {

	class opengl_shader : public shader
	{
	public:
		opengl_shader(const std::string &fpath);
		opengl_shader(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
		virtual ~opengl_shader(void);

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		inline virtual const std::string &get_name(void) const override { return m_name; }

		virtual void set_int(const std::string &name, int val) override;
		virtual void set_int_array(const std::string &name, int *vals, uint32_t count) override;

		virtual void set_float(const std::string &name, float val) override;
		virtual void set_float2(const std::string &name, const glm::vec2 &vec) override;
		virtual void set_float3(const std::string &name, const glm::vec3 &vec) override;
		virtual void set_float4(const std::string &name, const glm::vec4 &vec) override;

		virtual void set_mat3(const std::string &name, const glm::mat3 &mat) override;
		virtual void set_mat4(const std::string &name, const glm::mat4 &mat) override;

	public:
		void upload_uniform_int(const std::string &name, int val);
		void upload_uniform_int_array(const std::string &name, int *vals, uint32_t count);

		void upload_uniform_float(const std::string &name, float val);
		void upload_uniform_float2(const std::string &name, const glm::vec2 &vec);
		void upload_uniform_float3(const std::string &name, const glm::vec3 &vec);
		void upload_uniform_float4(const std::string &name, const glm::vec4 &vec);

		void upload_uniform_mat3(const std::string &name, const glm::mat3 &mat);
		void upload_uniform_mat4(const std::string &name, const glm::mat4 &mat);

	private:
		void compile(std::unordered_map<uint32_t /*GLenum*/, std::string> &shader_sources);

		int get_location(const std::string &name);

	private:
		uint32_t m_renderer_id;
		std::string m_name;
		std::unordered_map<std::string, int> m_location_map;
	};
}
