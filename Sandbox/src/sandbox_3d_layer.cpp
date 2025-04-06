#include "sandbox_3d_layer.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

sandbox_3d_layer::sandbox_3d_layer(void)
	: layer("Sandbox3D"), camera_controller(60.0f, 16.0f / 9.0f)
{
	this->camera_controller.set_position({ -1.5f, -1.5f, 1.5f });
	this->camera_controller.set_pitch_deg(-55.0f);
	this->camera_controller.set_yaw_deg(45.0f);

	auto font = elm::font::get_default();

	auto shader = elm::shader::create("content/shaders/texture_unit.glsl");
	this->pbr_shader = elm::shader::create("content/shaders/pbr.glsl");
	//auto unlit_generic_shader = elm::shader::create("content/shaders/unlit_generic.glsl");

	auto cube_mesh = elm::mesh::create("content/meshes/cube.obj");
	auto suzanne_mesh = elm::mesh::create("content/meshes/suzanne.obj");

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

	// -- Setup scene --
	this->scene = elm::scene::create();
	this->scene->set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	this->scene->set_show_world_grid(true);

	// Environment light
	{
		auto cubemap = elm::cubemap::create("content/textures/skybox/minedump_flats.hdr", 512);

		elm::entity entity = this->scene->create_entity();

		auto& env = entity.add_component<elm::environment_light_component>();
		env.irradiance_map = elm::cubemap::create_irradiance(cubemap, 32);
		env.prefilter_map = elm::cubemap::create_prefilter(cubemap, 128);
		env.brdf_lut_map = elm::cubemap::create_brdf_lut_map(512);
	}

	// Lights
	{
		this->dir_light = this->scene->create_entity();

		auto &light = this->dir_light.add_component<elm::directional_light_component>();
		light.direction = glm::normalize(glm::vec3(1, -1, -1));
		light.color = { 1.0f, 0.79f, 0.56f };
		light.intensity = 1.0f;
	}

	{
		this->point_light = this->scene->create_entity();
		
		auto &light = this->point_light.add_component<elm::point_light_component>();
		light.color = { 1.0f, 0.0f, 1.0f };
		light.intensity = 50.0f;

		auto &tc = this->point_light.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	}

	{
		elm::entity entity = this->scene->create_entity();

		auto &light = entity.add_component<elm::point_light_component>();
		light.color = { 0.0f, 1.0f, 0.0f };
		light.intensity = 50.0f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f });
	}

	// 2D sprites
	{
		elm::entity entity = this->scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::sprite_renderer_component>();
		circle_renderer.color = { 0.8f, 0.2f, 0.3f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, -1.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	{
		elm::entity entity = this->scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::sprite_renderer_component>();
		circle_renderer.color = { 0.8f, 0.2f, 0.3f, 1.0f };
		circle_renderer.texture = texture_checkerboard;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	{
		elm::entity entity = this->scene->create_entity();

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
		elm::entity entity = this->scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::circle_renderer_component>();
		circle_renderer.color = { 0.2f, 0.3f, 0.8f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 1.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f});
	}

	{
		elm::entity entity = this->scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::circle_renderer_component>();
		circle_renderer.color = { 0.2f, 0.8f, 0.3f, 1.0f };
		circle_renderer.thickness = 0.2f;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { -1.0f, 1.0f, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f });
	}

	// Text
	{
		elm::entity entity = this->scene->create_entity();

		auto &circle_renderer = entity.add_component<elm::text_renderer_component>();
		circle_renderer.text = "Sandbox example\nThis is on a new line!";
		circle_renderer.font = font;
		circle_renderer.color = { 0.2f, 0.8f, 0.3f, 1.0f };

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 1.25f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), { 1.0f, 0.0f, 0.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	}

	auto mat = std::make_shared<elm::pbr_material>();
	{
		auto roughness_map = elm::texture_2d::create({ .width = 1, .height = 1, .format = elm::image_format::RGB8, });
		uint32_t roughness_data = 0xFF000000;
		roughness_data |= (uint8_t)(0.15 * 255.0f);
		roughness_map->set_data(&roughness_data, 3);

		auto texture_black = elm::texture_2d::create({ .width = 1, .height = 1, .format = elm::image_format::RGB8, });
		uint32_t black_texture_data = 0xFF000000;
		texture_black->set_data(&black_texture_data, 3);

		auto texture_white = elm::texture_2d::create({ .width = 1, .height = 1, .format = elm::image_format::RGB8, });
		uint32_t white_texture_data = 0xFFFFFFFF;
		texture_white->set_data(&white_texture_data, 3);

		mat->shader = this->pbr_shader;
		mat->albedo = texture_checkerboard;
		mat->normal = texture_white; // TODO: Use a proper normal texture
		mat->roughness = roughness_map;
		mat->ao = texture_white;
		mat->metalness = texture_black;
	}

	// 3D meshes
	{
		elm::entity entity = this->scene->create_entity();

		auto &renderer = entity.add_component<elm::mesh_renderer_component>();
		renderer.mesh = cube_mesh;
		renderer.material = mat;

		auto &tc = entity.add_component<elm::transform_component>();
		tc.transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });
	}

	{
		this->suzanne = this->scene->create_entity();

		auto &renderer = this->suzanne.add_component<elm::mesh_renderer_component>();
		renderer.mesh = suzanne_mesh;
		renderer.material = mat;

		auto &tc = this->suzanne.add_component<elm::transform_component>();
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
	if (elm::input::key_pressed(elm::key::F5)) {
		this->pbr_shader->reload();
	}

	this->camera_controller.on_update(ts);

	{
		auto &tc = this->suzanne.get_component<elm::transform_component>();
		glm::vec3 pos, rot, scale;
		elm::math::decompose_transform(tc.transform, &pos, &rot, &scale);
		pos.z = glm::sin(elm::time::get_seconds()) * 0.5f;
		rot.z = glm::cos(elm::time::get_seconds());
		tc.transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), rot.z, { 0.0f, 0.0f, 1.0f });
	}

	{
		auto &tc = this->point_light.get_component<elm::transform_component>();
		glm::vec3 pos;
		elm::math::decompose_transform(tc.transform, &pos, nullptr, nullptr);
		pos.x = glm::cos(elm::time::get_seconds() * this->point_light_speed) * 5.0f;
		pos.y = glm::sin(elm::time::get_seconds() * this->point_light_speed) * 5.0f;
		tc.transform = glm::translate(glm::mat4(1.0f), pos);
	}

	elm::scene_renderer::render(this->scene, this->camera_controller.get_camera());
}

void sandbox_3d_layer::on_event(elm::event &e)
{
	this->camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);
	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sandbox_3d_layer::on_window_resize));
}

void sandbox_3d_layer::on_imgui_render(void)
{
	// -- Scene --
	ImGui::Begin("Scene");

	static bool show_world_grid = true;
	if (ImGui::Checkbox("Show world grid", &show_world_grid)) {
		this->scene->set_show_world_grid(show_world_grid);
	}

	ImGui::Text("Directional light");
	auto &dlc = this->dir_light.get_component<elm::directional_light_component>();
	ImGui::ColorEdit3("Color##DirectionalLight", glm::value_ptr(dlc.color));
	ImGui::DragFloat("Intensity##DirectionalLight", &dlc.intensity, 0.01f);

	ImGui::Text("Point light");
	auto &plc = this->point_light.get_component<elm::point_light_component>();
	ImGui::DragFloat("Speed##PointLight", &this->point_light_speed, 0.01f);
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
	this->camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
