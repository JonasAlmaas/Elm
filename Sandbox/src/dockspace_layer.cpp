#include "dockspace_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

dockspace_layer::dockspace_layer(void)
	: layer("Dockspace layer"), m_camera(-1.778f, 1.778f, -1, 1)
{
}

void dockspace_layer::on_attach(void)
{
	uint32_t checkerboard_data[8 * 8];
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			checkerboard_data[y * 8 + x] = (x + y) % 2 == 0 ? 0xFFFFFFFF : 0xFFCCCCCC;
		}
	}
	m_texture_checkerboard = elm::texture_2d::create(8, 8);
	m_texture_checkerboard->set_data((void *)checkerboard_data, sizeof checkerboard_data);

	m_frame_buffer = elm::frame_buffer::create({
		.width = 1280,
		.height = 720,
	});
}

void dockspace_layer::on_detach(void)
{
}

void dockspace_layer::on_update(elm::timestep ts)
{
	m_frame_buffer->bind();

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer_2d::begin_scene(&m_camera);

	static float s_rotation = 0.0f;
	s_rotation += 50.0f * ts.get_seconds();
	elm::renderer_2d::draw_rotated_quad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(s_rotation), m_texture_checkerboard, 5.0f);

	elm::renderer_2d::end_scene();

	m_frame_buffer->unbind();
}

void dockspace_layer::on_event(elm::event &e)
{
}

void dockspace_layer::on_imgui_render(void)
{
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "ALT+F4")) {
				elm::application::get()->close();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("My dockable window");
	ImGui::Image(m_texture_checkerboard->get_renderer_id(), { 256.0f, 256.0f });
	ImGui::End();

	ImGui::Begin("Viewport");
	ImGui::Image(m_frame_buffer->get_color_attachment_renderer_id(), { 1280.0f, 720.0f });
	ImGui::End();
}
