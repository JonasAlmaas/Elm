#pragma once

#include "elm/core/renderer/shader.h"
#include <stdint.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace elm {

	class opengl_shader : public shader
	{
	public:
		opengl_shader(const std::filesystem::path &fpath);
		opengl_shader(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
		virtual ~opengl_shader(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void reload(void) override;

		inline virtual const std::string &get_name(void) const override { return m_name; }

	private:
		void load_from_file(void);
		void delete_binaries(void);

		void compile_or_get_vulkan_binaries(std::unordered_map<uint32_t /*GLenum*/, std::string> &shader_sources);
		void compile_or_get_opengl_binaries(void);
		void create_program(void);
		void reflect(uint32_t /*GLenum*/ kind, const std::vector<uint32_t> &shader_data);

	private:
		uint32_t m_renderer_id;
		std::string m_name;
		std::filesystem::path m_fpath;

		std::unordered_map<uint32_t /*GLenum*/, std::vector<uint32_t>> m_vulkan_spirv;
		std::unordered_map<uint32_t /*GLenum*/, std::vector<uint32_t>> m_opengl_spirv;

		std::unordered_map<uint32_t /*GLenum*/, std::string> m_opengl_source_code;
	};
}
