#include "application.h"

#include <glad/glad.h>

namespace elm {

	application* application::s_instance = nullptr;

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

	application::application(void)
		: m_running(true)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = std::unique_ptr<window>(window::create());
		m_window->set_event_callback(BIND_EVENT_FN(application::on_event));

		m_imgui_layer = new imgui_layer();
		push_overlay(m_imgui_layer);

		// Hello triangle
		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		m_vertex_buffer.reset(vertex_buffer::create((void *)vertices, sizeof vertices));
		{
			vertex_buffer_layout layout = {
				{ shader_data_type::Float3, "a_position" },
				{ shader_data_type::Float4, "a_color" }};
			m_vertex_buffer->set_layout(&layout);
		}

		const vertex_buffer_layout *layout = m_vertex_buffer->get_layout();
		uint32_t ix = 0;
		for (const auto &el : *layout) {
			glEnableVertexAttribArray(ix);
			glVertexAttribPointer(
				ix,
				el.get_component_count(),
				shader_data_type_to_opengl_base_type(el.type),
				el.normalized ? GL_TRUE : GL_FALSE,
				layout->get_stride(),
				(const void *)el.offset);
			++ix;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		m_index_buffer.reset(index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));

		std::string vertex_src = R"(
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = vec4(a_position, 1.0);
}
)";

		std::string fragment_src = R"(
#version 330 core

layout(location = 0) out vec4 o_color;

in vec4 v_color;

void main()
{
	o_color = v_color;
}
)";

		m_shader = std::make_unique<shader>(vertex_src, fragment_src);
	}

	application::~application(void)
	{
	}

	void application::run(void)
	{
		while (m_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();
			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, m_index_buffer->get_count(), GL_UNSIGNED_INT, nullptr);

			for (auto layer : m_layer_stack) {
				layer->on_update();
			}

			m_imgui_layer->begin();
			for (auto layer : m_layer_stack) {
				layer->on_imgui_render();
			}
			m_imgui_layer->end();

			m_window->on_update();
		}
	}

	void application::push_layer(layer* layer)
	{
		m_layer_stack.push_layer(layer);
	}

	void application::push_overlay(layer* layer)
	{
		m_layer_stack.push_overlay(layer);
	}

	void application::on_event(event& e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.dispatch<window_close_event>(BIND_EVENT_FN(application::on_window_close));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.handled) {
				break;
			}
		}
	}

	bool application::on_window_close(window_close_event& e)
	{
		m_running = false;
		return true;
	}
}
