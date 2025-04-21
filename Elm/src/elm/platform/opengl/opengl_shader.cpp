#include "opengl_shader.hpp"
#include "elm/core/timer.hpp"
#include "elm/utils/crc32.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <fstream>
#include <filesystem>
#include <array>

namespace elm {

	static const std::filesystem::path s_cache_directory = "content/_shadercache/opengl";

	static void ensure_cache_directory_exists()
	{
		if (!std::filesystem::exists(s_cache_directory)) {
			std::filesystem::create_directories(s_cache_directory);
		}
	}

	static GLenum shader_kind_from_string(const std::string &kind_str)
	{
		if (kind_str == "vertex") {
			return GL_VERTEX_SHADER;
		} else if (kind_str == "fragment" || kind_str == "pixel") {
			return GL_FRAGMENT_SHADER;
		}

		ELM_CORE_ASSERT(false, "Unknown shader kind!");
		return 0;
	}

	static const char *glshader_kind_to_string(GLenum kind)
	{
		switch (kind) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}

		ELM_CORE_ASSERT(false, "Unknown shader kind!");
		return nullptr;
	}

	static shaderc_shader_kind glshader_kind_to_shaderc(GLenum kind)
	{
		switch (kind) {
		case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}

		ELM_CORE_ASSERT(false, "Unknown shader kind");
		return (shaderc_shader_kind)0;
	}

	static std::string read_file(const std::filesystem::path &fpath)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		std::string result;

		std::ifstream fs(fpath, std::ios::in | std::ios::binary);
		if (fs) {
			fs.seekg(0, std::ios::end);
			result.resize(fs.tellg());
			fs.seekg(0, std::ios::beg);
			fs.read(&result[0], result.size());
			fs.close();
		} else {
			ELM_CORE_ERROR("Could not open file \"{0}\"", fpath.string());
		}

		return result;
	}

	static std::unordered_map<GLenum, std::string> preprocess(const std::string &src)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		std::unordered_map<GLenum, std::string> result;

		const char *token_type = "#type";
		size_t token_type_leng = strlen(token_type);

		size_t pos = src.find(token_type, 0);
		while (pos != std::string::npos) {
			size_t eol = src.find_first_of('\n', pos);
			ELM_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + token_type_leng + 1;
			std::string type = src.substr(begin, eol - begin);
			GLenum shader_type = shader_kind_from_string(type);
			ELM_CORE_ASSERT(shader_type, "Invalid shader type specified");

			size_t next_line_pos = src.find_first_not_of('\n', eol);
			pos = src.find(token_type, next_line_pos);
			result[shader_type] = src.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? src.size() - 1 : next_line_pos));
		}

		return result;
	}

	static const char *glshader_kind_cached_vulkan_file_extension(uint32_t kind)
	{
		switch (kind) {
		case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
		case GL_FRAGMENT_SHADER: return ".cached_vulkan.pixel";
		}

		ELM_CORE_ASSERT(false, "Unknown shader kind!");
		return "";
	}

	static const char *glshader_kind_cached_opengl_file_extension(uint32_t kind)
	{
		switch (kind) {
		case GL_VERTEX_SHADER: return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER: return ".cached_opengl.pixel";
		}

		ELM_CORE_ASSERT(false, "Unknown shader kind!");
		return "";
	}

	opengl_shader::opengl_shader(const std::filesystem::path &fpath)
		: renderer_id(UINT32_MAX), fpath(fpath), name(fpath.stem().string())
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		ensure_cache_directory_exists();
		load_from_file();
	}

	opengl_shader::opengl_shader(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src)
		: renderer_id(UINT32_MAX), name(name), fpath(name)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		ensure_cache_directory_exists();

		std::unordered_map<GLenum, std::string> shader_sources = {
			{ GL_VERTEX_SHADER, vertex_src },
			{ GL_FRAGMENT_SHADER, fragment_src }};
		timer timer;
		ELM_CORE_INFO("Compiling shader \"{0}\"...", this->name);
		compile_or_get_vulkan_binaries(shader_sources);
		compile_or_get_opengl_binaries();
		create_program();
		ELM_CORE_INFO("Shader compilation of \"{0}\" complete in {1} ms", this->name, timer.elapsed_milliseconds());
	}

	opengl_shader::~opengl_shader()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(this->renderer_id);
	}

	void opengl_shader::bind()
	{
		glUseProgram(this->renderer_id);
	}

	void opengl_shader::unbind()
	{
		glUseProgram(0);
	}

	void opengl_shader::reload()
	{
		delete_binaries();
		load_from_file(); // TODO: Only do it is there is a valid fpath
	}

	void opengl_shader::load_from_file()
	{
		std::string shader_source = read_file(this->fpath);
		auto shader_sources = preprocess(shader_source);

		timer timer;
		ELM_CORE_INFO("Compiling shader \"{0}\"...", this->name);
		compile_or_get_vulkan_binaries(shader_sources);
		compile_or_get_opengl_binaries();
		create_program();
		ELM_CORE_INFO("Shader compilation of \"{0}\" complete in {1} ms", this->name, timer.elapsed_milliseconds());
	}

	void opengl_shader::delete_binaries()
	{
		if (std::filesystem::exists(s_cache_directory)) {
			for (auto &kind : { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }) {
				std::filesystem::path cachedOpenGLPath = s_cache_directory / (this->fpath.filename().string() + glshader_kind_cached_opengl_file_extension(kind));
				std::filesystem::path cachedVulkanPath = s_cache_directory / (this->fpath.filename().string() + glshader_kind_cached_vulkan_file_extension(kind));
				remove(cachedOpenGLPath);
				remove(cachedVulkanPath);
			}
		}
	}

	void opengl_shader::compile_or_get_vulkan_binaries(std::unordered_map<GLenum, std::string> &shader_sources)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		const bool optimize = false; // TODO: Expose this maybe?
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		this->vulkan_spirv.clear();

		for (auto &&[kind, source] : shader_sources) {
			bool has_valid_cache = false;
			uint32_t checksum_from_source = crc32((uint8_t *)source.data(), (uint32_t)source.size());

			std::filesystem::path shader_fpath = this->fpath;
			std::filesystem::path cache_path = s_cache_directory / (shader_fpath.filename().string() + glshader_kind_cached_vulkan_file_extension(kind));

			std::ifstream in(cache_path, std::ios::in | std::ios::binary);
			if (in.is_open()) { // Has cachced shader
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				uint32_t checksum_from_file;
				in.read((char *)&checksum_from_file, sizeof(checksum_from_file));

				has_valid_cache = checksum_from_file == checksum_from_source;

				if (has_valid_cache) {
					size -= sizeof(checksum_from_file);
					auto &data = this->vulkan_spirv[kind];
					data.resize(size / sizeof(uint32_t));
					in.read((char *)data.data(), size);
				}
			}

			if (!has_valid_cache) {
				shaderc::SpvCompilationResult res = compiler.CompileGlslToSpv(
					source,
					glshader_kind_to_shaderc(kind),
					(const char *)this->fpath.c_str(),
					options);
				if (res.GetCompilationStatus() != shaderc_compilation_status_success) {
					ELM_CORE_ERROR(res.GetErrorMessage());
					ELM_CORE_ASSERT(false, "Shader Compilation Failed!");
				}

				this->vulkan_spirv[kind] = std::vector<uint32_t>(res.cbegin(), res.cend());

				std::ofstream out(cache_path, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto &data = this->vulkan_spirv[kind];
					out.write((const char *)&checksum_from_source, sizeof(checksum_from_source));
					out.write((char *)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto &&[kind, data] : this->vulkan_spirv) {
			reflect(kind, data);
		}
	}

	void opengl_shader::compile_or_get_opengl_binaries()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		const bool optimize = false; // TODO: Expose this maybe?
		if (optimize) {
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		this->opengl_spirv.clear();

		for (auto &&[kind, spirv] : this->vulkan_spirv) {
			bool has_valid_cache = false;
			uint32_t checksum_from_spirv = crc32((uint8_t *)spirv.data(), (uint32_t)(spirv.size() * sizeof(uint32_t)));

			std::filesystem::path shader_fpath = this->fpath;
			std::filesystem::path cache_path = s_cache_directory / (shader_fpath.filename().string() + glshader_kind_cached_opengl_file_extension(kind));

			std::ifstream in(cache_path, std::ios::in | std::ios::binary);
			if (in.is_open()) { // Has cachced shader
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				uint32_t checksum_from_file;
				in.read((char *)&checksum_from_file, sizeof(checksum_from_file));

				has_valid_cache = checksum_from_file == checksum_from_spirv;

				if (has_valid_cache) {
					size -= sizeof(checksum_from_file);
					auto &data = this->opengl_spirv[kind];
					data.resize(size / sizeof(uint32_t));
					in.read((char *)data.data(), size);
				}
			}

			if (!has_valid_cache) {
				spirv_cross::CompilerGLSL glsl_compiler(spirv);
				auto source = glsl_compiler.compile(); // OpenGL source code

				shaderc::SpvCompilationResult res = compiler.CompileGlslToSpv(
					source,
					glshader_kind_to_shaderc(kind),
					(const char *)this->fpath.c_str(),
					options);
				if (res.GetCompilationStatus() != shaderc_compilation_status_success) {
					ELM_CORE_ERROR(res.GetErrorMessage());
					ELM_CORE_ASSERT(false, "Shader Compilation Failed!");
				}

				this->opengl_spirv[kind] = std::vector<uint32_t>(res.cbegin(), res.cend());

				std::ofstream out(cache_path, std::ios::out | std::ios::binary);
				if (out.is_open()) {
					auto &data = this->opengl_spirv[kind];
					out.write((const char *)&checksum_from_spirv, sizeof(checksum_from_spirv));
					out.write((char *)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void opengl_shader::create_program()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLuint program = glCreateProgram();

		std::vector<GLuint> shader_ids;
		for (auto &&[kind, spirv] : this->opengl_spirv) {
			GLuint shader_id = shader_ids.emplace_back(glCreateShader(kind));
			glShaderBinary(1, &shader_id, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)(spirv.size() * sizeof(uint32_t)));
			glSpecializeShader(shader_id, "main", 0, nullptr, nullptr);
			glAttachShader(program, shader_id);
		}

		glLinkProgram(program);

		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&is_linked);
		if (is_linked == GL_FALSE) {
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

			glDeleteProgram(program);
			for (auto shader : shader_ids) {
				glDeleteShader(shader);
			}

			ELM_CORE_ERROR("{0}", info_log.data());
			ELM_CORE_ASSERT(false, "Shader link failure");

			return;
		}

		for (auto shader : shader_ids) {
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		this->renderer_id = program;
	}

	void opengl_shader::reflect(uint32_t kind, const std::vector<uint32_t> &shader_data)
	{
		spirv_cross::Compiler compiler(shader_data);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		ELM_CORE_TRACE("opengl_shader::reflect - {0} {1}", glshader_kind_to_string(kind), this->fpath.string());
		ELM_CORE_TRACE("  {0} uniform buffers", resources.uniform_buffers.size());
		ELM_CORE_TRACE("  {0} resources", resources.sampled_images.size());

		ELM_CORE_TRACE("Uniform buffers:");
		for (const auto &resource : resources.uniform_buffers) {
			const auto &buf_type = compiler.get_type(resource.base_type_id);
			size_t buf_size = compiler.get_declared_struct_size(buf_type);
			size_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			size_t member_count = buf_type.member_types.size();

			ELM_CORE_TRACE("  {0}", resource.name);
			ELM_CORE_TRACE("    Size = {0}", buf_size);
			ELM_CORE_TRACE("    Binding = {0}", binding);
			ELM_CORE_TRACE("    Members = {0}", member_count);
		}
	}
}
