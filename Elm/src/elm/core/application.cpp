#include "application.h"

#include <glad/glad.h>

namespace elm {

	application* application::s_instance = nullptr;

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

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};

		m_vertex_buffer.reset(vertex_buffer::create((void *)vertices, sizeof vertices));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		uint32_t indices[3] = { 0, 1, 2 };
		m_index_buffer.reset(index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));

		std::string vertex_src = R"(
#version 330 core

layout(location = 0) in vec3 a_position;

out vec3 v_position;

void main()
{
	v_position = a_position;
	gl_Position = vec4(a_position, 1.0);
}
)";

		std::string fragment_src = R"(
#version 330 core

layout(location = 0) out vec4 o_color;
in vec3 v_position;

void main()
{
	o_color = vec4(v_position.xy + 0.5, 0.0, 1.0);
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
