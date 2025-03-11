#include "sandbox_3d_layer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

sandbox_3d_layer::sandbox_3d_layer(void)
	: layer("Sandbox3D"), m_camera_controller(60.0f, 16.0f / 9.0f)
{
	m_camera_controller.set_position({ -1.5f, -1.5f, 1.5f });
	m_camera_controller.set_pitch_deg(-55.0f);
	m_camera_controller.set_yaw_deg(45.0f);

	m_shader = elm::shader::create("content/shaders/texture.glsl");

	uint32_t checkerboard_data[8 * 8];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			checkerboard_data[y * 8 + x] = (x + y) % 2 == 0 ? 0xFFFFFFFF : 0xFFCCCCCC;
		}
	}
	m_texture_checkerboard = elm::texture_2d::create(8, 8, {
		.mag_filter = elm::texture_2d_filter::NEAREST
	});
	m_texture_checkerboard->set_data((void *)checkerboard_data, sizeof checkerboard_data);

	// Setup vertex array to render
	m_vertex_array = elm::vertex_array::create();
	float vertices[5 * 4 * 6] = {
		// Front face
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		// Left face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		// Right face
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

		// Top face
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,

		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f
	};

	auto vb = elm::vertex_buffer::create((void *)vertices, sizeof vertices);
	elm::vertex_buffer_layout layout = {
		{ elm::shader_data_type::Float3, "a_position" },
		{ elm::shader_data_type::Float2, "a_uv" } };
	vb->set_layout(&layout);
	m_vertex_array->add_vertex_buffer(vb);

	uint32_t indices[6 * 6] = {
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Back face
		4, 5, 6,
		6, 7, 4,
		// Left face
		8, 9, 10,
		10, 11, 8,
		// Right face
		12, 13, 14,
		14, 15, 12,
		// Top face
		16, 17, 18,
		18, 19, 16,
		// Bottom face
		20, 21, 22,
		22, 23, 20
	};
	auto ib = elm::index_buffer::create(indices, sizeof indices / sizeof(uint32_t));
	m_vertex_array->set_index_buffer(ib);

	// World grid
	m_world_grid_shader = elm::shader::create("content/shaders/world_grid.glsl");
	m_world_grid_ub = elm::uniform_buffer::create(sizeof m_world_grid_data, 1);

	// Setup scene
	/*m_scene = elm::scene::create();
	elm::entity entity = m_scene->create_entity();
	auto &transform = entity.add_component<elm::transform_component>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));*/
}

void sandbox_3d_layer::on_attach(void)
{
}

void sandbox_3d_layer::on_detach(void)
{
}

void sandbox_3d_layer::on_update(elm::timestep ts)
{
	if (elm::input::is_key_pressed(elm::key::F5)) {
		m_world_grid_shader->reload();
	}

	m_camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer::begin_scene(m_camera_controller.get_camera());

	m_texture_checkerboard->bind();
	elm::renderer::submit(m_shader, m_vertex_array);

	m_world_grid_shader->bind();
	m_world_grid_data.camera_position = m_camera_controller.get_position();
	m_world_grid_ub->bind();
	m_world_grid_ub->set_data((const void *)&m_world_grid_data, sizeof m_world_grid_data);
	elm::render_command::draw_arrays(6);

	elm::renderer::end_scene();

	// TODO: Use the scene renderer
	//elm::scene_renderer::render(m_scene, &m_camera);
}

void sandbox_3d_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sandbox_3d_layer::on_window_resize));
}

void sandbox_3d_layer::on_imgui_render(void)
{
	// -- Renderer --
	ImGui::Begin("Renderer");

	static std::string vendor = elm::render_command::get_vendor();
	static std::string renderer = elm::render_command::get_render();
	static std::string renderer_version = elm::render_command::get_render_version();

	ImGui::Text("Vendor: %s", vendor.c_str());
	ImGui::Text("Renderer: %s", renderer.c_str());
	ImGui::Text("Version: %s", renderer_version.c_str());
	ImGui::Text("Frame time: %.3fms", elm::application::get()->get_telemetry()->get_smooth_frame_time_s() * 1000.0f);
	ImGui::Text("FPS: %.2f", elm::application::get()->get_telemetry()->get_fps());

	ImGui::End();

	// -- Misc --
	ImGui::Begin("Misc");

	auto camera_pos = m_camera_controller.get_position();
	if (ImGui::DragFloat3("Camera position", glm::value_ptr(camera_pos), 0.1f)) {
		m_camera_controller.set_position(camera_pos);
	}
	float pitch = m_camera_controller.get_pitch_deg();
	if (ImGui::DragFloat("Camera pitch", &pitch, 0.01f)) {
		m_camera_controller.set_pitch_deg(pitch);
	}
	float yaw = m_camera_controller.get_yaw_deg();
	if (ImGui::DragFloat("Camera yaw", &yaw, 0.01f)) {
		m_camera_controller.set_yaw_deg(yaw);
	}
	float roll = m_camera_controller.get_roll_deg();
	if (ImGui::DragFloat("Camera roll", &roll, 0.01f)) {
		m_camera_controller.set_roll_deg(roll);
	}

	ImGui::End();
}

bool sandbox_3d_layer::on_window_resize(elm::window_resize_event &e)
{
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
