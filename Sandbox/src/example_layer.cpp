#include "example_layer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

// Temporary
#include <elm/platform/opengl/opengl_shader.h>

example_layer::example_layer(void)
	: layer("ExampleLayer"),
	m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
	// Triangle
	{
		m_triangle_va = elm::vertex_array::create();

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f};

		auto vb = std::shared_ptr<elm::vertex_buffer>(elm::vertex_buffer::create((void *)vertices, sizeof vertices));
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" }};
		vb->set_layout(&layout);
		m_triangle_va->add_vertex_buffer(vb);

		uint32_t indices[3] = { 0, 1, 2 };
		auto ib = std::shared_ptr<elm::index_buffer>(elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
		m_triangle_va->set_index_buffer(ib);
	}

	// Square
	{
		m_square_va = elm::vertex_array::create();

		float vertices[5 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f};

		auto vb = std::shared_ptr<elm::vertex_buffer>(elm::vertex_buffer::create((void *)vertices, sizeof vertices));
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" } };
		vb->set_layout(&layout);
		m_square_va->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = std::shared_ptr<elm::index_buffer>(elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t)));
		m_square_va->set_index_buffer(ib);
	}

	{
		std::string vertex_src = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;

			uniform mat4 u_view_projection;
			uniform mat4 u_transform;

			out vec4 v_color;

			void main()
			{
				gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
			}
			)";

		std::string fragment_src = R"(
			#version 330 core

			layout(location = 0) out vec4 o_color;

			uniform vec4 u_color;

			void main()
			{
				o_color = u_color;
			}
			)";

		m_flat_color_shader = elm::shader::create(vertex_src, fragment_src);
	}

	{
		std::string vertex_src = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_uv;

			uniform mat4 u_view_projection;
			uniform mat4 u_transform;

			out vec2 v_uv;

			void main()
			{
				v_uv = a_uv;
				gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
			}
			)";

		std::string fragment_src = R"(
			#version 330 core

			layout(location = 0) out vec4 o_color;

			in vec2 v_uv;

			uniform sampler2D u_texture;

			void main()
			{
				o_color = texture(u_texture, v_uv);
			}
			)";

		m_texture_shader = elm::shader::create(vertex_src, fragment_src);

		m_texture = elm::texture_2d::create("content/textures/dev/checkerboard.png");

		std::dynamic_pointer_cast<elm::opengl_shader>(m_texture_shader)->bind();
		std::dynamic_pointer_cast<elm::opengl_shader>(m_texture_shader)->upload_uniform_int("u_texture", 0);
	}
}

void example_layer::on_update(elm::timestep ts)
{
	if (elm::input::is_key_pressed(elm::key::A)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(-m_camera_move_speed * ts.get_seconds(), 0.0f, 0.0f));
	} else if (elm::input::is_key_pressed(elm::key::D)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(m_camera_move_speed * ts.get_seconds(), 0.0f, 0.0f));
	}

	if (elm::input::is_key_pressed(elm::key::W)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(0.0f, m_camera_move_speed * ts.get_seconds(), 0.0f));
	} else if (elm::input::is_key_pressed(elm::key::S)) {
		m_camera.set_position(m_camera.get_position() + glm::vec3(0.0f, -m_camera_move_speed * ts.get_seconds(), 0.0f));
	}

	if (elm::input::is_key_pressed(elm::key::Q)) {
		m_camera.set_rotation_deg(m_camera.get_rotation_deg() - m_camera_rot_speed * ts.get_seconds());
	} else if (elm::input::is_key_pressed(elm::key::E)) {
		m_camera.set_rotation_deg(m_camera.get_rotation_deg() + m_camera_rot_speed * ts.get_seconds());
	}

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	elm::render_command::clear();

	elm::renderer::begin_scene(&m_camera);

	glm::vec4 color_red(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 color_blue(0.2f, 0.3f, 0.8f, 1.0f);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 20; ++x) {
			glm::vec3 pos(x * 0.2f, y * 0.2f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			if ((y % 2 == 0 && x % 2 == 0)
					|| (y % 2 != 0 && x % 2 != 0)) {
				std::dynamic_pointer_cast<elm::opengl_shader>(m_flat_color_shader)->upload_uniform_float4("u_color", color_red);
			} else {
				std::dynamic_pointer_cast<elm::opengl_shader>(m_flat_color_shader)->upload_uniform_float4("u_color", color_blue);
			}
			elm::renderer::submit(m_flat_color_shader, m_square_va, transform);
		}
	}

	m_texture->bind();
	elm::renderer::submit(m_texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	elm::renderer::submit(m_flat_color_shader, m_triangle_va);

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

	ImGui::DragFloat("Camera move speeed", &m_camera_move_speed, 0.1f, 0.1f, 5.0f);
	ImGui::DragFloat("Camera rotation speeed", &m_camera_rot_speed, 0.5f, 0.5f, 100.0f);

	ImGui::End();
}

bool example_layer::on_key_pressed(elm::key_pressed_event &e)
{
	return false;
}
