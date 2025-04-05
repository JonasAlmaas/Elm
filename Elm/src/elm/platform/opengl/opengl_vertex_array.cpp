#include "opengl_vertex_array.hpp"

#include <glad/glad.h>

namespace elm {

	static GLenum shader_data_type_to_opengl_base_type(shader_data_type type)
	{
		switch (type) {
		case elm::shader_data_type::BOOL:
			return GL_BOOL;
		case elm::shader_data_type::INT:
		case elm::shader_data_type::INT2:
		case elm::shader_data_type::INT3:
		case elm::shader_data_type::INT4:
			return GL_INT;
		case elm::shader_data_type::FLOAT:
		case elm::shader_data_type::FLOAT2:
		case elm::shader_data_type::FLOAT3:
		case elm::shader_data_type::FLOAT4:
		case elm::shader_data_type::MAT3:
		case elm::shader_data_type::MAT4:
			return GL_FLOAT;
		}

		ELM_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	opengl_vertex_array::opengl_vertex_array()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glGenVertexArrays(1, &this->renderer_id);
	}

	opengl_vertex_array::~opengl_vertex_array()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &this->renderer_id);
	}

	void opengl_vertex_array::bind(void)
	{
		glBindVertexArray(this->renderer_id);
	}

	void opengl_vertex_array::unbind(void)
	{
		glBindVertexArray(0);
	}

	void opengl_vertex_array::add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glBindVertexArray(this->renderer_id);
		vertex_buffer->bind();

		const auto layout = vertex_buffer->get_layout();
		ELM_CORE_ASSERT(layout->get_elements().size(), "Unable to add vertex buffer without layout to vertex array");

		uint32_t vb_ix = 0;
		for (const auto &el : *layout) {
			switch (el.type) {
			case shader_data_type::FLOAT:
			case shader_data_type::FLOAT2:
			case shader_data_type::FLOAT3:
			case shader_data_type::FLOAT4:
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
			case shader_data_type::INT:
			case shader_data_type::INT2:
			case shader_data_type::INT3:
			case shader_data_type::INT4:
			case shader_data_type::BOOL:
				glEnableVertexAttribArray(vb_ix);
				glVertexAttribIPointer(
					vb_ix,
					el.get_component_count(),
					shader_data_type_to_opengl_base_type(el.type),
					layout->get_stride(),
					(const void *)el.offset);
				++vb_ix;
				break;
			case shader_data_type::MAT3:
			case shader_data_type::MAT4:
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

		this->vbs.push_back(vertex_buffer);
	}

	void opengl_vertex_array::set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		this->ib = index_buffer;

		glBindVertexArray(this->renderer_id);
		this->ib->bind();
	}
}
