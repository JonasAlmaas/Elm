#include "example_layer.h"

#include <imgui.h>

example_layer::example_layer(void)
	: layer("ExampleLayer"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f)
{
	// Triangle
	{
		m_vertex_array.reset(elm::vertex_array::create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		auto vb = std::shared_ptr<elm::vertex_buffer>(elm::vertex_buffer::create((void *)vertices, sizeof vertices));
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float4, "a_color" } };
		vb->set_layout(&layout);
		m_vertex_array->add_vertex_buffer(vb);

		uint32_t indices[3] = { 0, 1, 2 };
		auto ib = std::shared_ptr<elm::index_buffer>(elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
		m_vertex_array->set_index_buffer(ib);
	}

	// Square
	{
		m_vertex_array2.reset(elm::vertex_array::create());

		float vertices[7 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.75f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		};

		auto vb = std::shared_ptr<elm::vertex_buffer>(elm::vertex_buffer::create((void *)vertices, sizeof vertices));
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float4, "a_color" } };
		vb->set_layout(&layout);
		m_vertex_array2->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = std::shared_ptr<elm::index_buffer>(elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
		m_vertex_array2->set_index_buffer(ib);
	}

	std::string vertex_src = R"(
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_view_projection;

out vec4 v_color;

void main()
{
	v_color = a_color;
	gl_Position = u_view_projection * vec4(a_position, 1.0);
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

	m_shader = std::make_unique<elm::shader>(vertex_src, fragment_src);
}

void example_layer::on_update(void)
{
	if (elm::input::is_key_pressed(elm::key::A)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(-m_camera_move_speed, 0.0f, 0.0f));
	} else if (elm::input::is_key_pressed(elm::key::D)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(m_camera_move_speed, 0.0f, 0.0f));
	}

	if (elm::input::is_key_pressed(elm::key::W)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(0.0f, m_camera_move_speed, 0.0f));
	} else if (elm::input::is_key_pressed(elm::key::S)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(0.0f, -m_camera_move_speed, 0.0f));
	}

	if (elm::input::is_key_pressed(elm::key::Q)) {
		m_camera.set_rotation_deg(m_camera.get_rotation_deg() - m_camera_rot_speed);
	} else if (elm::input::is_key_pressed(elm::key::E)) {
		m_camera.set_rotation_deg(m_camera.get_rotation_deg() + m_camera_rot_speed);
	}

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	elm::render_command::clear();

	elm::renderer::begin_scene(&m_camera);

	elm::renderer::submit(m_shader, m_vertex_array2);
	elm::renderer::submit(m_shader, m_vertex_array);

	elm::renderer::end_scene();
}

void example_layer::on_event(elm::event &e)
{
	elm::event_dispatcher dispatcher(e);
	dispatcher.dispatch<elm::key_pressed_event>(ELM_BIND_EVENT_FN(example_layer::on_key_pressed));
}

void example_layer::on_imgui_render(void)
{
	ImGui::Begin("Test");

	ImGui::DragFloat("Camera move speeed", &m_camera_move_speed, 0.01f, 0.005f, 0.25f);
	ImGui::DragFloat("Camera rotation speeed", &m_camera_rot_speed, 0.05f, 0.01f, 10.0f);

	ImGui::End();
}

bool example_layer::on_key_pressed(elm::key_pressed_event &e)
{
	return false;
}
