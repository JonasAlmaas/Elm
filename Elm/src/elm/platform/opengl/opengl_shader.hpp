#pragma once

#include "elm/core/renderer/shader.hpp"
#include <stdint.h>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace elm {

	struct opengl_shader : shader
	{
		opengl_shader(const std::filesystem::path &fpath);
		opengl_shader(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src);
		virtual ~opengl_shader(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void reload(void) override;

		inline virtual const std::string &get_name(void) const override { return this->name; }

	private:
		void load_from_file(void);
		void delete_binaries(void);

		void compile_or_get_vulkan_binaries(std::unordered_map<uint32_t /*GLenum*/, std::string> &shader_sources);
		void compile_or_get_opengl_binaries(void);
		void create_program(void);
		void reflect(uint32_t /*GLenum*/ kind, const std::vector<uint32_t> &shader_data);

	private:
		uint32_t renderer_id;
		std::string name;
		std::filesystem::path fpath;

		std::unordered_map<uint32_t /*GLenum*/, std::vector<uint32_t>> vulkan_spirv;
		std::unordered_map<uint32_t /*GLenum*/, std::vector<uint32_t>> opengl_spirv;
	};
}
