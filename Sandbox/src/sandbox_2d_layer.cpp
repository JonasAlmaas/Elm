#include "sandbox_2d_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

sandbox_2d_layer::sandbox_2d_layer(void)
	: layer("Sandbox2D"), m_camera_controller(16.0f / 9.0f, true)
{
}

void sandbox_2d_layer::on_attach(void)
{
	m_texture_grass_tileset = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png", {
		.mag_filter = elm::texture_2d_filter::NEAREST
	});

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
}

void sandbox_2d_layer::on_detach(void)
{
}

void sandbox_2d_layer::on_update(elm::timestep ts)
{
	m_frame_time_acc += ts.get_seconds();
	++m_frame_time_acc_ix;
	if (m_frame_time_acc_ix >= 30) {
		m_avg_frame_delta = m_frame_time_acc / (float)m_frame_time_acc_ix;
		m_frame_time_acc = 0.0f;
		m_frame_time_acc_ix = 0;
	}

	m_camera_controller.on_update(ts);

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer_2d::reset_stats();
	elm::renderer_2d::begin_scene(m_camera_controller.get_camera());

	for (int y = 0; y < 200; ++y) {
		for (int x = 0; x < 200; ++x) {
			elm::renderer_2d::draw_quad(
				{ 2.0f + x, 2.0f + y },
				{ 1.0f, 1.0f },
				(x + y) % 2 == 0
					? glm::vec4(0.8f, 0.2f, 0.3f, 1.0f)
					: glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		}
	}

	elm::renderer_2d::draw_quad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	elm::renderer_2d::draw_quad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	elm::renderer_2d::draw_quad({ 0.0f, 0.0f }, { 1.5f, 1.0f }, m_texture_grass_tileset);

	static float s_rotation = 0.0f;
	s_rotation += 50.0f * ts.get_seconds();
	elm::renderer_2d::draw_rotated_quad({ 2.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(s_rotation), m_texture_checkerboard, 5.0f);

	elm::renderer_2d::draw_circle(
		glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 0.0f)),
		glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

	elm::renderer_2d::draw_line({ -1.0f, -1.0f }, { 0.0f, -2.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });

	elm::renderer_2d::end_scene();
}

void sandbox_2d_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sandbox_2d_layer::on_window_resize));
}

void sandbox_2d_layer::on_imgui_render(void)
{
	elm::renderer_2d::statistics stats = elm::renderer_2d::get_stats();

	ImGui::Begin("Statistics");
	ImGui::Text("FPS: %.2f", 1.0f / m_avg_frame_delta);
	ImGui::Text("Quad count: %d", stats.quad_count);
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
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
