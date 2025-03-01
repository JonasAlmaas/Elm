#include "application.h"

#include <glad/glad.h>

namespace elm {

	Application* Application::s_instance = nullptr;

	Application::Application(void)
		: m_running(true)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(Application::on_event));

		m_imgui_layer = new ImGuiLayer();
		push_overlay(m_imgui_layer);

		// Hello triangle
		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		glGenBuffers(1, &m_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		uint32_t indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

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

		m_shader = std::make_unique<Shader>(vertex_src, fragment_src);
	}

	Application::~Application(void)
	{
	}

	void Application::run(void)
	{
		while (m_running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();
			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

	void Application::push_layer(Layer* layer)
	{
		m_layer_stack.push_layer(layer);
	}

	void Application::push_overlay(Layer* layer)
	{
		m_layer_stack.push_overlay(layer);
	}

	void Application::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::on_window_close));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.handled) {
				break;
			}
		}
	}

	bool Application::on_window_close(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}
