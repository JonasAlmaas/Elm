#include "opengl_vertex_array.h"

#include <glad/glad.h>

namespace elm {

	static GLenum shader_data_type_to_opengl_base_type(shader_data_type type)
	{
		switch (type) {
		case elm::shader_data_type::Bool:
			return GL_BOOL;
		case elm::shader_data_type::Int:
		case elm::shader_data_type::Int2:
		case elm::shader_data_type::Int3:
		case elm::shader_data_type::Int4:
			return GL_INT;
		case elm::shader_data_type::Float:
		case elm::shader_data_type::Float2:
		case elm::shader_data_type::Float3:
		case elm::shader_data_type::Float4:
		case elm::shader_data_type::Mat3:
		case elm::shader_data_type::Mat4:
			return GL_FLOAT;
		}

		ELM_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	opengl_vertex_array::opengl_vertex_array()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glGenVertexArrays(1, &m_renderer_id);
	}

	opengl_vertex_array::~opengl_vertex_array()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void opengl_vertex_array::bind(void)
	{
		glBindVertexArray(m_renderer_id);
	}

	void opengl_vertex_array::unbind(void)
	{
		glBindVertexArray(0);
	}

	void opengl_vertex_array::add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_renderer_id);
		vertex_buffer->bind();

		const auto layout = vertex_buffer->get_layout();
		ELM_CORE_ASSERT(layout->get_elements().size(), "Unable to add vertex buffer without layout to vertex array");

		uint32_t vb_ix = 0;
		for (const auto &el : *layout) {
			switch (el.type) {
			case shader_data_type::Float:
			case shader_data_type::Float2:
			case shader_data_type::Float3:
			case shader_data_type::Float4:
				glEnableVertexAttribArray(vb_ix);
				glVertexAttribPointer(
					vb_ix,
					el.get_component_count(),
					shader_data_type_to_opengl_base_type(el.type),
					el.normalized ? GL_TRUE : GL_FALSE,
					layout->get_stride(),
					(const void*)el.offset);
				++vb_ix;
				break;
			case shader_data_type::Int:
			case shader_data_type::Int2:
			case shader_data_type::Int3:
			case shader_data_type::Int4:
			case shader_data_type::Bool:
				glEnableVertexAttribArray(vb_ix);
				glVertexAttribIPointer(
					vb_ix,
					el.get_component_count(),
					shader_data_type_to_opengl_base_type(el.type),
					layout->get_stride(),
					(const void *)el.offset);
				++vb_ix;
				break;
			case shader_data_type::Mat3:
			case shader_data_type::Mat4:
			{
				uint32_t count = el.get_component_count();
				for (uint32_t i = 0; i < count; ++i) {
					glEnableVertexAttribArray(vb_ix);
					glVertexAttribPointer(
						vb_ix,
						count,
						shader_data_type_to_opengl_base_type(el.type),
						el.normalized ? GL_TRUE : GL_FALSE,
						layout->get_stride(),
						(const void *)(sizeof(float) * count * i));
					glVertexAttribDivisor(vb_ix, 1);
					++vb_ix;
				}
				break;
			}
			default:
				ELM_CORE_ASSERT(false, "Unknown shader_data_type");
				break;
			}
		}

		m_vertex_buffers.push_back(vertex_buffer);
	}

	void opengl_vertex_array::set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(m_renderer_id);
		index_buffer->bind();

		m_index_buffer = index_buffer;
	}
}
