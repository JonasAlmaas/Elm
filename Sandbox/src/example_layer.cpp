#include "example_layer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

example_layer::example_layer(void)
	: layer("ExampleLayer"), m_camera_controller(16.0f / 9.0f, true)
{
	// Triangle
	{
		m_triangle_va = elm::vertex_array::create();

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f};

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" }};
		vb->set_layout(&layout);
		m_triangle_va->add_vertex_buffer(vb);

		uint32_t indices[3] = { 0, 1, 2 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
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

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::Float3, "a_position" },
			{ elm::shader_data_type::Float2, "a_uv" } };
		vb->set_layout(&layout);
		m_square_va->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
		m_square_va->set_index_buffer(ib);
	}

	m_flat_color_shader = m_shader_library.load("content/shaders/flat_color.glsl");
	m_texture_shader = m_shader_library.load("content/shaders/texture.glsl");

	m_texture = elm::texture_2d::create("content/textures/dev/checkerboard.png");
	m_texture2 = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png");

	m_texture_shader->bind();
	m_texture_shader->set_int("u_texture", 0);
}

void example_layer::on_update(elm::timestep ts)
{
	m_camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	elm::render_command::clear();

	elm::renderer::begin_scene(m_camera_controller.get_camera());

	glm::vec4 color_red(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 color_blue(0.2f, 0.3f, 0.8f, 1.0f);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 20; ++x) {
			glm::vec3 pos(x * 0.2f, y * 0.2f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			m_flat_color_shader->bind();
			m_flat_color_shader->set_float4("u_color", (y%2==0 && x%2==0) || (y%2!=0 && x%2!=0) ? color_red : color_blue);
			elm::renderer::submit(m_flat_color_shader, m_square_va, transform);
		}
	}

	m_texture->bind();
	elm::renderer::submit(m_texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_texture2->bind();
	elm::renderer::submit(m_texture_shader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	elm::renderer::submit(m_flat_color_shader, m_triangle_va);

	elm::renderer::end_scene();
}

void example_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(example_layer::on_window_resize));
}

void example_layer::on_imgui_render(void)
{
}

bool example_layer::on_window_resize(elm::window_resize_event &e)
{
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
