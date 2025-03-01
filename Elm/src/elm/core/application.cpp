#include "application.h"

#include "elm/core/renderer/renderer.h"
#include "elm/core/renderer/render_command.h"

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

		// Triangle
		{
			m_vertex_array.reset(vertex_array::create());

			float vertices[7 * 3] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			};

			auto vb = std::shared_ptr<vertex_buffer>(vertex_buffer::create((void *)vertices, sizeof vertices));
			vertex_buffer_layout layout = {
				{ shader_data_type::Float3, "a_position" },
				{ shader_data_type::Float4, "a_color" }};
			vb->set_layout(&layout);
			m_vertex_array->add_vertex_buffer(vb);

			uint32_t indices[3] = { 0, 1, 2 };
			auto ib = std::shared_ptr<index_buffer>(index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
			m_vertex_array->set_index_buffer(ib);
		}

		// Square
		{
			m_vertex_array2.reset(vertex_array::create());

			float vertices[7 * 4] = {
				-0.75f, -0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				 0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				-0.75f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			};

			auto vb = std::shared_ptr<vertex_buffer>(vertex_buffer::create((void *)vertices, sizeof vertices));
			vertex_buffer_layout layout = {
				{ shader_data_type::Float3, "a_position" },
				{ shader_data_type::Float4, "a_color" } };
			vb->set_layout(&layout);
			m_vertex_array2->add_vertex_buffer(vb);

			uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
			auto ib = std::shared_ptr<index_buffer>(index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
			m_vertex_array2->set_index_buffer(ib);
		}

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
			for (auto layer : m_layer_stack) {
				layer->on_update();
			}

			render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
			render_command::clear();

			renderer::begin_scene();

			m_shader->bind();
			renderer::submit(m_vertex_array2);

			m_shader->bind();
			renderer::submit(m_vertex_array);

			renderer::end_scene();

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
