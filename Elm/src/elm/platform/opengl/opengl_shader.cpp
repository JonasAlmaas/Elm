#include "opengl_shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <fstream>
#include <array>

namespace elm {

	static GLenum shader_type_from_string(const std::string &type)
	{
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		} else if (type == "fragment" || type == "pixel") {
			return GL_FRAGMENT_SHADER;
		}

		ELM_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static std::string read_file(const std::string &fpath)
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
			ELM_CORE_ERROR("Could not open file \"{}\"", fpath);
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
			GLenum shader_type = shader_type_from_string(type);
			ELM_CORE_ASSERT(shader_type, "Invalid shader type specified");

			size_t next_line_pos = src.find_first_not_of('\n', eol);
			pos = src.find(token_type, next_line_pos);
			result[shader_type] = src.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? src.size() - 1 : next_line_pos));
		}

		return result;
	}

	opengl_shader::opengl_shader(const std::string &fpath)
		: m_renderer_id(UINT32_MAX)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		std::string shader_source = read_file(fpath);
		auto shader_sources = preprocess(shader_source);
		compile(shader_sources);

		// Extract name from fpath
		// "content/shaders/flat_color.glsl" = "flat_color"
		size_t last_slash = fpath.find_last_of("/\\");
		last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
		size_t last_dot = fpath.rfind('.');
		size_t count = last_dot == std::string::npos
			? fpath.size() - last_slash
			: last_dot - last_slash;
		m_name = fpath.substr(last_slash, count);
	}

	opengl_shader::opengl_shader(
			const std::string &name,
			const std::string &vertex_src,
			const std::string &fragment_src)
		: m_renderer_id(UINT32_MAX), m_name(name)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		std::unordered_map<GLenum, std::string> shader_sources = {
			{ GL_VERTEX_SHADER, vertex_src },
			{ GL_FRAGMENT_SHADER, fragment_src }};
		compile(shader_sources);
	}

	opengl_shader::~opengl_shader(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteProgram(m_renderer_id);
	}

	void opengl_shader::bind(void) const
	{
		glUseProgram(m_renderer_id);
	}

	void opengl_shader::unbind(void) const
	{
		glUseProgram(0);
	}

	void opengl_shader::compile(std::unordered_map<GLenum, std::string> &shader_sources)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		ELM_CORE_ASSERT(shader_sources.size() <= 2, "More than two shader sources are not supported");
		std::array<GLenum, 2> glshader_ids;
		int glshader_id_ix = 0;

		GLuint program = glCreateProgram();

		for (auto &kv : shader_sources) {
			GLenum type = kv.first;
			const std::string &src = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar *src_c_str = (const GLchar *)src.c_str();
			glShaderSource(shader, 1, &src_c_str, 0);

			glCompileShader(shader);

			GLint is_compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
			if (is_compiled == GL_FALSE) {
				GLint max_length = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> info_log(max_length);
				glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

				glDeleteShader(shader);

				ELM_CORE_ERROR("{0}", info_log.data());
				ELM_CORE_ASSERT(false, "Shader compilation failure");

				break;
			}

			glAttachShader(program, shader);
			glshader_ids[glshader_id_ix++] = shader;
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
			for (auto shader : glshader_ids) {
				glDeleteShader(shader);
			}

			ELM_CORE_ERROR("{0}", info_log.data());
			ELM_CORE_ASSERT(false, "Shader link failure");

			return;
		}

		for (auto shader : glshader_ids) {
			glDetachShader(program, shader);
			glDeleteShader(shader);
		}

		m_renderer_id = program;
	}

	int opengl_shader::get_location(const std::string &name)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (m_location_map.find(name) != m_location_map.end()) {
			return m_location_map[name];
		} else {
			GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
			m_location_map[name] = location;
			return location;
		}
	}

	void opengl_shader::set_int(const std::string &name, int val)
	{
		upload_uniform_int(name, val);
	}

	void opengl_shader::set_float(const std::string &name, float val)
	{
		upload_uniform_float(name, val);
	}

	void opengl_shader::set_float2(const std::string &name, const glm::vec2 &vec)
	{
		upload_uniform_float2(name, vec);
	}

	void opengl_shader::set_float3(const std::string &name, const glm::vec3 &vec)
	{
		upload_uniform_float3(name, vec);
	}

	void opengl_shader::set_float4(const std::string &name, const glm::vec4 &vec)
	{
		upload_uniform_float4(name, vec);
	}

	void opengl_shader::set_mat3(const std::string &name, const glm::mat3 &mat)
	{
		upload_uniform_mat3(name, mat);
	}

	void opengl_shader::set_mat4(const std::string &name, const glm::mat4 &mat)
	{
		upload_uniform_mat4(name, mat);
	}

	void opengl_shader::upload_uniform_int(const std::string &name, int val)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniform1i(location, val);
	}

	void opengl_shader::upload_uniform_float(const std::string &name, float val)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniform1f(location, val);
	}

	void opengl_shader::upload_uniform_float2(const std::string &name, const glm::vec2 &vec)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniform2f(location, vec.x, vec.y);
	}

	void opengl_shader::upload_uniform_float3(const std::string &name, const glm::vec3 &vec)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void opengl_shader::upload_uniform_float4(const std::string &name, const glm::vec4 &vec)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}

	void opengl_shader::upload_uniform_mat3(const std::string &name, const glm::mat3 &mat)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void opengl_shader::upload_uniform_mat4(const std::string &name, const glm::mat4 &mat)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint location = get_location(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}
