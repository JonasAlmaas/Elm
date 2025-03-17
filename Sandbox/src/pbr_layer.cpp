#include "pbr_layer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

pbr_layer::pbr_layer(void)
	: layer("Sandbox3D"), m_camera_controller(60.0f, 16.0f / 9.0f)
{
}

void pbr_layer::on_attach(void)
{
	m_camera_controller.set_position({ -1.5f, -1.5f, 1.5f });
	m_camera_controller.set_pitch_deg(-55.0f);
	m_camera_controller.set_yaw_deg(45.0f);

	m_pbr_shader = elm::shader::create("content/shaders/pbr.glsl");

	auto cube_mesh = elm::mesh::create("content/meshes/cube.obj");
	auto sphere_mesh = elm::mesh::create("content/meshes/sphere.obj");

	uint32_t checkerboard_data[8 * 8];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			checkerboard_data[y * 8 + x] = (x + y) % 2 == 0 ? 0xFFFFFFFF : 0xFFCCCCCC;
		}
	}
	auto texture_checkerboard = elm::texture_2d::create({
		.width = 8,
		.height = 8,
		.format = elm::image_format::RGBA8,
		.mag_filter = elm::texture_filter::NEAREST
	});
	texture_checkerboard->set_data((void *)checkerboard_data, sizeof checkerboard_data);

	auto white_texture = elm::texture_2d::create({ .width = 1, .height = 1, .format = elm::image_format::RGBA8, });
	uint32_t white_texture_data = 0xFFFFFFFF;
	white_texture->set_data(&white_texture_data, sizeof white_texture_data);

	auto black_texture = elm::texture_2d::create({ .width = 1, .height = 1, .format = elm::image_format::RGBA8, });
	uint32_t black_texture_data = 0xFF000000;
	black_texture->set_data(&black_texture_data, sizeof black_texture_data);

	// -- Setup scene --
	m_scene = elm::scene::create();
	m_scene->set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	m_scene->set_show_world_grid(false);

	// Environment light
	{
		auto cubemap = elm::cubemap::create("content/textures/skybox/minedump_flats.hdr", 512);

		elm::entity entity = m_scene->create_entity();

		auto& env = entity.add_component<elm::environment_light_component>();
		env.irradiance_map = elm::cubemap::create_irradiance(cubemap, 32);
		env.prefilter_map = elm::cubemap::create_prefilter(cubemap, 128);
		env.brdf_lut_map = elm::cubemap::create_brdf_lut_map(512);
	}

	// Directional light
	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::directional_light_component>();
		light.direction = glm::normalize(glm::vec3(1, -1, -1));
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 1.0f;
	}

	// Point lights
	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 10.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { -4.0f, -4.0f, 2.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 10.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 4.0f, -4.0f, 2.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 10.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 4.0f, 4.0f, 2.0f });
	}

	{
		elm::entity entity = m_scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 10.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { -4.0f, 4.0f, 2.0f });
	}

	// Meshes
	{
		auto mat = std::make_shared<elm::pbr_material>();
		mat->shader = m_pbr_shader;
		mat->albedo = texture_checkerboard;
		mat->normal = white_texture;
		mat->rougness = black_texture;
		mat->ao = white_texture;
		mat->metalness = black_texture;

		elm::entity entity = m_scene->create_entity();

		auto &renderer = entity.add_component<elm::mesh_renderer_component>();
		//renderer.mesh = cube_mesh;
		renderer.mesh = sphere_mesh;
		renderer.material = mat;

		entity.add_component<elm::transform_component>(glm::mat4(1.0f));
	}
}

void pbr_layer::on_detach(void)
{
}

void pbr_layer::on_update(elm::timestep ts)
{
	if (elm::input::is_key_pressed(elm::key::F5)) {
		m_pbr_shader->reload();
	}

	m_camera_controller.on_update(ts);

	elm::scene_renderer::render(m_scene, m_camera_controller.get_camera());
}

void pbr_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(pbr_layer::on_window_resize));
}

void pbr_layer::on_imgui_render(void)
{
	// -- Scene --
	ImGui::Begin("Scene");

	static bool show_world_grid = false;
	if (ImGui::Checkbox("Show world grid", &show_world_grid)) {
		m_scene->set_show_world_grid(show_world_grid);
	}

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

bool pbr_layer::on_window_resize(elm::window_resize_event &e)
{
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
