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

	auto font = elm::font::get_default();

	auto shader = elm::shader::create("content/shaders/texture_unit.glsl");
	m_pbr_shader = elm::shader::create("content/shaders/pbr.glsl");
	//auto unlit_generic_shader = elm::shader::create("content/shaders/unlit_generic.glsl");

	auto cube_mesh = elm::mesh::create("content/meshes/cube.obj");
	auto suzanne_mesh = elm::mesh::create("content/meshes/suzanne.obj");

	uint32_t checkerboard_data[8 * 8];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			checkerboard_data[y * 8 + x] = (x + y) % 2 == 0 ? 0xFFFFFFFF : 0xFFCCCCCC;
		}
	}
	auto texture_checkerboard = elm::texture_2d::create(8, 8, {
		.format = elm::image_format::RGBA8,
		.mag_filter = elm::texture_2d_filter::NEAREST
	});
	texture_checkerboard->set_data((void *)checkerboard_data, sizeof checkerboard_data);

	// -- Setup scene --
	m_scene = elm::scene::create();
	m_scene->set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	m_scene->set_show_world_grid(true);

	// Lights
	{
		m_dir_light = m_scene->create_entity();

		auto &light = m_dir_light.add_component<elm::directional_light_component>();
		light.direction = glm::normalize(glm::vec3(1, -1, -1));
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 1.0f;
		light.ambient_color = { 0.54f, 0.78f, 1.0f };
		light.ambient_intensity = 0.4f;
	}

	{
		m_point_light = m_scene->create_entity();
		
		auto &light = m_point_light.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.0f, 1.0f };
		light.intensity = 1.0f;

		auto &tc = m_point_light.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 0.0f, 1.0f, 0.0f };
		light.intensity = 1.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	}

	// 2D sprites
	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::sprite_renderer_component>();
		circle_renderer.color = { 0.8f, 0.2f, 0.3f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, -1.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::sprite_renderer_component>();
		circle_renderer.color = { 0.8f, 0.2f, 0.3f, 1.0f };
		circle_renderer.texture = texture_checkerboard;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::sprite_renderer_component>();
		circle_renderer.color = { 0.8f, 0.2f, 0.3f, 1.0f };
		circle_renderer.texture = texture_checkerboard;
		circle_renderer.tiling_factor = { 1.0f, 2.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, 1.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	// 2D circles
	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::circle_renderer_component>();
		circle_renderer.color = { 0.2f, 0.3f, 0.8f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 1.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f});
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::circle_renderer_component>();
		circle_renderer.color = { 0.2f, 0.8f, 0.3f, 1.0f };
		circle_renderer.thickness = 0.2f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { -1.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	// Text
	{
		elm::entity entity = m_scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::text_renderer_component>();
		circle_renderer.text = "Sandbox example\nThis is on a new line!";
		circle_renderer.font = font;
		circle_renderer.color = { 0.2f, 0.8f, 0.3f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 1.25f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	}

	// 3D meshes
	{
		elm::entity entity = m_scene->create_entity();

		auto &renderer = entity.add_component<elm::mesh_renderer_component>();
		renderer.mesh = cube_mesh;
		renderer.shader = m_pbr_shader;
		renderer.textures.push_back(texture_checkerboard);

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });
	}

	{
		m_suzanne = m_scene->create_entity();

		auto &renderer = m_suzanne.add_component<elm::mesh_renderer_component>();
		renderer.mesh = suzanne_mesh;
		renderer.shader = m_pbr_shader;
		renderer.textures.push_back(texture_checkerboard);

		auto &tc = m_suzanne.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 2.0f, 0.0f, 0.0f });
	}
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
		m_pbr_shader->reload();
	}

	m_camera_controller.on_update(ts);

	{
		auto &tc = m_suzanne.get_component<elm::transform_component>();
		glm::vec3 pos, rot, scale;
		elm::math::decompose_transform(tc.transform, &pos, &rot, &scale);
		pos.z = glm::sin(elm::time::get_seconds()) * 0.5f;
		rot.z = glm::cos(elm::time::get_seconds());
		tc.transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), rot.z, { 0.0f, 0.0f, 1.0f });
	}

	{
		auto &tc = m_point_light.get_component<elm::transform_component>();
		glm::vec3 pos;
		elm::math::decompose_transform(tc.transform, &pos, nullptr, nullptr);
		pos.x = glm::cos(elm::time::get_seconds() * m_point_light_speed) * 5.0f;
		pos.y = glm::sin(elm::time::get_seconds() * m_point_light_speed) * 5.0f;
		tc.transform = glm::translate(glm::mat4(1.0f), pos);
	}

	elm::scene_renderer::render(m_scene, m_camera_controller.get_camera());
}

void sandbox_3d_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sandbox_3d_layer::on_window_resize));
}

void sandbox_3d_layer::on_imgui_render(void)
{
	// -- Scene --
	ImGui::Begin("Scene");

	static bool show_world_grid = true;
	if (ImGui::Checkbox("Show world grid", &show_world_grid)) {
		m_scene->set_show_world_grid(show_world_grid);
	}

	ImGui::Text("Directional light");
	auto &dlc = m_dir_light.get_component<elm::directional_light_component>();
	ImGui::ColorEdit3("Color##DirectionalLight", glm::value_ptr(dlc.color));
	ImGui::DragFloat("Intensity##DirectionalLight", &dlc.intensity, 0.01f);
	ImGui::ColorEdit3("Ambient color##DirectionalLight", glm::value_ptr(dlc.ambient_color));
	ImGui::DragFloat("Ambient intensity##DirectionalLight", &dlc.ambient_intensity, 0.01f);

	ImGui::Text("Point light");
	auto &plc = m_point_light.get_component<elm::point_light_component>();
	ImGui::DragFloat("Speed##PointLight", &m_point_light_speed, 0.01f);
	ImGui::ColorEdit3("Color##PointLight", glm::value_ptr(plc.color));
	ImGui::DragFloat("Intensity##PointLight", &plc.intensity, 0.01f);

	ImGui::End();

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
}

bool sandbox_3d_layer::on_window_resize(elm::window_resize_event &e)
{
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
