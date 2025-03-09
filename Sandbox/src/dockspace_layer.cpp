#include "dockspace_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

dockspace_layer::dockspace_layer(void)
	: layer("Dockspace layer"), m_camera_controller(16.0f / 9.0f, false)
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
		.attachments = {
			elm::frame_buffer_texture_format::RGBA8,
			elm::frame_buffer_texture_format::DEPTH24STENCIL8
		}
	});
}

void dockspace_layer::on_detach(void)
{
}

void dockspace_layer::on_update(elm::timestep ts)
{
	if (auto spec = m_frame_buffer->get_spec();
			m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f
			&& (spec->width != (uint32_t)m_viewport_size.x || spec->height != (uint32_t)m_viewport_size.y)) {
		m_frame_buffer->resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_camera_controller.resize_viewport((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
	}

	if (m_viewport_focused) {
		m_camera_controller.on_update(ts);
	}

	m_frame_buffer->bind();

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer_2d::begin_scene(m_camera_controller.get_camera());

	elm::renderer_2d::draw_quad({ -0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	elm::renderer_2d::draw_quad({ 0.5f, 0.5f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	static float s_rotation = 0.0f;
	s_rotation += 50.0f * ts.get_seconds();
	elm::renderer_2d::draw_rotated_quad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(s_rotation), m_texture_checkerboard, 5.0f);

	elm::renderer_2d::end_scene();

	m_frame_buffer->unbind();
}

void dockspace_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);
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

	// -- My dockable window --
	ImGui::Begin("My dockable window");

	ImGui::Image(m_texture_checkerboard->get_renderer_id(), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
	static char aaa[100] = {0};
	ImGui::InputText("Text", aaa, 100);

	float aspect_ratio = m_viewport_size.y / m_viewport_size.x;
	ImGui::Image(m_frame_buffer->get_depth_attachment_renderer_id(), { 256.0f, 256.0f * aspect_ratio }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

	ImGui::End();

	// -- Viewport --
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::Begin("Viewport");

	m_viewport_focused = ImGui::IsWindowFocused();
	m_viewport_hovered = ImGui::IsWindowHovered();
	elm::application::get()->get_imgui_layer()->set_block_events(!m_viewport_focused || !m_viewport_hovered);

	ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
	m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };
	ImGui::Image(m_frame_buffer->get_color_attachment_renderer_id(0), { m_viewport_size.x, m_viewport_size.y }, {0.0f, 1.0f}, {1.0f, 0.0f});

	ImGui::End();
	ImGui::PopStyleVar();
}
