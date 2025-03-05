#include "sandbox_2d_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

sandbox_2d_layer::sandbox_2d_layer(void)
	: layer("Sandbox2D"), m_camera_controller(16.0f / 9.0f, true)
{
}

void sandbox_2d_layer::on_attach(void)
{
	m_texture = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png");
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

	elm::renderer_2d::begin_scene(m_camera_controller.get_camera());

	for (int y = 0; y < 100; ++y) {
		for (int x = 0; x < 1000; ++x) {
			elm::renderer_2d::draw_quad(
				{ 2.0f + x, 2.0f + y },
				{ 1.0f, 1.0f },
				(y % 2 == 0 && x % 2 == 0) || (y % 2 != 0 && x % 2 != 0)
					? glm::vec4(0.8f, 0.2f, 0.3f, 1.0f)
					: glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
		}
	}

	elm::renderer_2d::draw_quad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	elm::renderer_2d::draw_quad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	elm::renderer_2d::draw_quad({ 0.0f, 0.0f }, { 1.5f, 1.0f }, m_texture);

	static float s_rotation = 0.0f;
	s_rotation += 50.0f * ts.get_seconds();
	elm::renderer_2d::draw_rotated_quad({ 2.0f, 0.0f }, { 1.5f, 1.0f }, glm::radians(s_rotation), m_texture);

	elm::renderer_2d::end_scene();
}

void sandbox_2d_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);
}

void sandbox_2d_layer::on_imgui_render(void)
{
	ImGui::Begin("Statistics");
	ImGui::Text("FPS: %.3f", 1.0f / m_avg_frame_delta);
	ImGui::End();
}
