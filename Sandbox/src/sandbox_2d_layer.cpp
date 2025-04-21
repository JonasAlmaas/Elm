#include "sandbox_2d_layer.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

sandbox_2d_layer::sandbox_2d_layer()
	: layer("Sandbox2D"), camera_controller(16.0f / 9.0f, true)
{
}

void sandbox_2d_layer::on_attach()
{
	this->texture_grass_tileset = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png", {
		.mag_filter = elm::texture_filter::NEAREST
	});

	uint32_t checkerboard_data[8 * 8];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			checkerboard_data[y * 8 + x] = (x + y) % 2 == 0 ? 0xFFFFFFFF : 0xFFCCCCCC;
		}
	}
	this->texture_checkerboard = elm::texture_2d::create({
		.width = 8,
		.height = 8,
		.mag_filter = elm::texture_filter::NEAREST
	});
	this->texture_checkerboard->set_data((void *)checkerboard_data, sizeof checkerboard_data);

	this->spline.points.push_back({-0.5f, 0.5f, 0.1f});
	this->spline.points.push_back({ -1.5f, -0.5f, 0.1f });
	this->spline.points.push_back({ 0.5f, -0.5f, 0.1f });
	this->spline.points.push_back({ 0.0f, 0.5f, 0.1f });
	this->spline.points.push_back({ 0.5f, 1.0f, 0.1f });
	this->spline.looped = false;
	this->spline.invalidate();

	for (int i = 0; i < 10; i++) {
		this->spline2.points.push_back({
			0.8f * sinf((float)i / 10.0f * 3.14159f * 2.0f),
			0.8f * cosf((float)i / 10.0f * 3.14159f * 2.0f),
			0.1f });
	}
	this->spline2.looped = true;
	this->spline2.invalidate();
}

void sandbox_2d_layer::on_detach()
{
}

void sandbox_2d_layer::on_update(elm::timestep ts)
{
	this->frame_time_acc += ts.get_seconds();
	++this->frame_time_acc_ix;
	if (this->frame_time_acc_ix >= 30) {
		this->avg_frame_delta = this->frame_time_acc / (float)this->frame_time_acc_ix;
		this->frame_time_acc = 0.0f;
		this->frame_time_acc_ix = 0;
	}

	this->camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer_2d::reset_stats();
	elm::renderer_2d::begin_scene(this->camera_controller.get_camera());

	for (int y = 0; y < 50; ++y) {
		for (int x = 0; x < 50; ++x) {
			elm::renderer_2d::draw_sprite(
				{ 2.0f + x, 2.0f + y },
				{ 1.0f, 1.0f },
				(x + y) % 2 == 0
					? glm::vec4(0.8f, 0.2f, 0.3f, 1.0f)
					: glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		}
	}

	elm::renderer_2d::draw_sprite({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	elm::renderer_2d::draw_sprite({ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	elm::renderer_2d::draw_sprite({ -2.0f, 0.0f }, { 1.5f, 1.0f }, this->texture_grass_tileset);

	static float s_rotation = 0.0f;
	s_rotation += 50.0f * ts.get_seconds();
	elm::renderer_2d::draw_rotated_sprite({ 2.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(s_rotation), this->texture_checkerboard, 5.0f);

	elm::renderer_2d::draw_circle(
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 0.0f)),
		glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

	elm::renderer_2d::draw_line({ -1.0f, -1.0f }, { 0.0f, -2.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });

	{
		for (auto &p : this->spline.points) {
			elm::renderer_2d::draw_sprite({ p.x, p.y, 0.2f }, { 0.01f, 0.01f });
		}

		glm::vec3 prev_p = this->spline.get_point(0.0f);
		const float step_size = 0.1f;
		for (float t = step_size; t <= this->spline.length + step_size; t += step_size) {
			glm::vec3 p = this->spline.get_point(this->spline.get_normalised_offset(t));
			elm::renderer_2d::draw_line(prev_p, p, { 1.0f, 0.0f, 1.0f, 1.0f });
			prev_p = p;
		}
	}

	{
		for (auto &p : this->spline2.points) {
			elm::renderer_2d::draw_sprite({ p.x, p.y, 0.2f }, { 0.01f, 0.01f });
		}

		glm::vec3 prev_p = this->spline2.get_point(0.0f);
		const float step_size = 0.1f;
		for (float t = step_size; t <= this->spline2.length; t += step_size) {
			glm::vec3 p = this->spline2.get_point(this->spline2.get_normalised_offset(t));
			elm::renderer_2d::draw_line(prev_p, p, { 0.0f, 1.0f, 0.0f, 1.0f });
			prev_p = p;
		}
	}

	elm::renderer_2d::end_scene();
}

void sandbox_2d_layer::on_event(elm::event &e)
{
	this->camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);
	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sandbox_2d_layer::on_window_resize));
}

void sandbox_2d_layer::on_imgui_render()
{
	elm::renderer_2d::statistics stats = elm::renderer_2d::get_stats();

	ImGui::Begin("Statistics");
	ImGui::Text("FPS: %.2f", 1.0f / this->avg_frame_delta);
	ImGui::Text("Draw calls: %d", stats.draw_calls);

	/*uint32_t mem_usage = stats.get_memory_usage();
	if (mem_usage < 1000) {
		ImGui::Text("Memory usage: %d B", mem_usage);
	} else if (mem_usage < 1'000'000) {
		ImGui::Text("Memory usage: %.2f KB", (float)mem_usage / 1000.0f);
	} else if (mem_usage < 1'000'000'000) {
		ImGui::Text("Memory usage: %.2f MB", (float)mem_usage / 1'000'000.0f);
	} else {
		ImGui::Text("Memory usage: %.2f GB", (float)mem_usage / 1'000'000'000.0f);
	}*/

	ImGui::End();
}

bool sandbox_2d_layer::on_window_resize(elm::window_resize_event &e)
{
	this->camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
