#include "example_layer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

example_layer::example_layer(void)
	: layer("ExampleLayer"), camera_controller(16.0f / 9.0f, true)
{
	// Triangle
	{
		this->triangle_va = elm::vertex_array::create();

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f};

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::FLOAT3, "a_position" }};
		vb->set_layout(&layout);
		this->triangle_va->add_vertex_buffer(vb);

		uint32_t indices[3] = { 0, 1, 2 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
		this->triangle_va->set_index_buffer(ib);
	}

	// Square
	{
		this->square_va = elm::vertex_array::create();

		float vertices[5 * 4] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f};

		auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
		elm::vertex_buffer_layout layout = {
			{ elm::shader_data_type::FLOAT3, "a_position" },
			{ elm::shader_data_type::FLOAT2, "a_uv" } };
		vb->set_layout(&layout);
		this->square_va->add_vertex_buffer(vb);

		uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
		auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
		this->square_va->set_index_buffer(ib);
	}

	this->flat_color_shader = this->shader_library.load("content/shaders/flat_color.glsl");
	this->texture_shader = this->shader_library.load("content/shaders/texture_unit.glsl");

	this->texture = elm::texture_2d::create("content/textures/dev/checkerboard.png", {
		.mag_filter = elm::texture_filter::NEAREST
	});
	this->texture2 = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png", {
		.mag_filter = elm::texture_filter::NEAREST
	});

	this->color_uniform_buffer = elm::uniform_buffer::create(sizeof(glm::vec4), 2);
}

void example_layer::on_update(elm::timestep ts)
{
	this->camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1 });
	elm::render_command::clear();

	elm::renderer::begin_scene(this->camera_controller.get_camera());

	glm::vec4 color_red(0.8f, 0.2f, 0.3f, 1.0f);
	glm::vec4 color_blue(0.2f, 0.3f, 0.8f, 1.0f);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 20; ++x) {
			if ((x + y) % 2 == 0) {
				this->color_uniform_buffer->set_data(&color_red, sizeof(color_red));
			} else {
				this->color_uniform_buffer->set_data(&color_blue, sizeof(color_blue));
			}
			glm::vec3 pos(x * 0.2f, y * 0.2f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			elm::renderer::submit(this->flat_color_shader, this->square_va, transform);
		}
	}

	this->texture->bind();
	elm::renderer::submit(this->texture_shader, this->square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	this->texture2->bind();
	elm::renderer::submit(this->texture_shader, this->square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	elm::renderer::submit(this->flat_color_shader, this->triangle_va);

	elm::renderer::end_scene();
}

void example_layer::on_event(elm::event &e)
{
	this->camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);
	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(example_layer::on_window_resize));
}

void example_layer::on_imgui_render(void)
{
}

bool example_layer::on_window_resize(elm::window_resize_event &e)
{
	this->camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
