#include "opengl_shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace elm {

	opengl_shader::opengl_shader(const std::string &vertex_src, const std::string &fragment_src)
		: m_renderer_id(UINT32_MAX)
	{
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar *source = (const GLchar *)vertex_src.c_str();
		glShaderSource(vertex_shader, 1, &source, 0);

		glCompileShader(vertex_shader);

		GLint is_compiled = 0;
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE) {
			GLint max_length = 0;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);

			glDeleteShader(vertex_shader);

			ELM_CORE_ERROR("{0}", info_log.data());
			ELM_CORE_ASSERT(false, "Vertex shader compilation error");

			return;
		}

		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		source = (const GLchar *)fragment_src.c_str();
		glShaderSource(fragment_shader, 1, &source, 0);

		glCompileShader(fragment_shader);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE) {
			GLint max_length = 0;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);

			glDeleteShader(fragment_shader);
			glDeleteShader(vertex_shader);

			ELM_CORE_ERROR("{0}", info_log.data());
			ELM_CORE_ASSERT(false, "Fragment shader compilation error");

			return;
		}

		m_renderer_id = glCreateProgram();

		glAttachShader(m_renderer_id, vertex_shader);
		glAttachShader(m_renderer_id, fragment_shader);

		glLinkProgram(m_renderer_id);

		GLint is_linked = 0;
		glGetProgramiv(m_renderer_id, GL_LINK_STATUS, (int *)&is_linked);
		if (is_linked == GL_FALSE) {
			GLint max_length = 0;
			glGetProgramiv(m_renderer_id, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(m_renderer_id, max_length, &max_length, &info_log[0]);

			glDeleteProgram(m_renderer_id);
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			ELM_CORE_ERROR("{0}", info_log.data());
			ELM_CORE_ASSERT(false, "Shader link failure");

			return;
		}

		glDetachShader(m_renderer_id, vertex_shader);
		glDetachShader(m_renderer_id, fragment_shader);
	}

	opengl_shader::~opengl_shader(void)
	{
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

	int opengl_shader::get_location(const std::string &name)
	{
		if (m_location_map.find(name) != m_location_map.end()) {
			return m_location_map[name];
		} else {
			GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
			m_location_map[name] = location;
			return location;
		}
	}

	void opengl_shader::upload_uniform_int(const std::string &name, int val)
	{
		GLint location = get_location(name);
		glUniform1i(location, val);
	}

	void opengl_shader::upload_uniform_float(const std::string &name, float val)
	{
		GLint location = get_location(name);
		glUniform1f(location, val);
	}

	void opengl_shader::upload_uniform_float2(const std::string &name, const glm::vec2 &vec)
	{
		GLint location = get_location(name);
		glUniform2f(location, vec.x, vec.y);
	}

	void opengl_shader::upload_uniform_float3(const std::string &name, const glm::vec3 &vec)
	{
		GLint location = get_location(name);
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void opengl_shader::upload_uniform_float4(const std::string &name, const glm::vec4 &vec)
	{
		GLint location = get_location(name);
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}

	void opengl_shader::upload_uniform_mat3(const std::string &name, const glm::mat3 &mat)
	{
		GLint location = get_location(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void opengl_shader::upload_uniform_mat4(const std::string &name, const glm::mat4 &mat)
	{
		GLint location = get_location(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}
}
