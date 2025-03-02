#include "sandbox_2d_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

// Temporary
#include <elm/platform/opengl/opengl_shader.h>

sandbox_2d_layer::sandbox_2d_layer(void)
	: layer("Sandbox2D"), m_camera_controller(16.0f / 9.0f, true)
{
}

void sandbox_2d_layer::on_attach(void)
{
	m_vertex_array = elm::vertex_array::create();

	float vertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f};

	auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
	elm::vertex_buffer_layout layout = {
		{ elm::shader_data_type::Float3, "a_position" }};
	vb->set_layout(&layout);
	m_vertex_array->add_vertex_buffer(vb);

	uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
	auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
	m_vertex_array->set_index_buffer(ib);

	m_shader = elm::shader::create("content/shaders/flat_color.glsl");
}

void sandbox_2d_layer::on_detach(void)
{
}

void sandbox_2d_layer::on_update(elm::timestep ts)
{
	m_camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	elm::render_command::clear();

	elm::renderer::begin_scene(m_camera_controller.get_camera());

	m_shader->bind();
	std::dynamic_pointer_cast<elm::opengl_shader>(m_shader)->upload_uniform_float4("u_color", glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
	elm::renderer::submit(m_shader, m_vertex_array, glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.15f, 0.0f)));

	m_shader->bind();
	std::dynamic_pointer_cast<elm::opengl_shader>(m_shader)->upload_uniform_float4("u_color", m_color);
	elm::renderer::submit(m_shader, m_vertex_array);

	elm::renderer::end_scene();
}

void sandbox_2d_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);
}

void sandbox_2d_layer::on_imgui_render(void)
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_color));
	ImGui::End();
}
