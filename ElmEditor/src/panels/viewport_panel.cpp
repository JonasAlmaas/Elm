#include "viewport_panel.hpp"
#include <imgui.h>

namespace elm {

	viewport_panel::viewport_panel()
		//: camera_controller(16.0f / 9.0f, false)
		: camera_controller(60.0f, 1.0f)
	{
	}

	void viewport_panel::on_attach()
	{
		this->camera_controller.set_position({ -1.5f, -1.5f, 1.5f });
		this->camera_controller.set_pitch_deg(-65.0f);
		this->camera_controller.set_yaw_deg(45.0f);

		frame_buffer_specification fb_spec;
		fb_spec.width = 1;
		fb_spec.height = 1;
		fb_spec.attachments = {
			elm::frame_buffer_texture_format::RGBA8,
			elm::frame_buffer_texture_format::DEPTH24STENCIL8
		};
		//fb_spec.samples = 8; // TODO: Fix multi sampling
		this->frame_buffer = elm::frame_buffer::create(fb_spec);
	}

	void viewport_panel::on_update(timestep ts, const std::shared_ptr<scene> &scene)
	{
		if (auto spec = this->frame_buffer->get_spec();
				this->viewport_size.x>0.0f && this->viewport_size.y>0.0f
				&& (spec->width!=(uint32_t)this->viewport_size.x || spec->height!=(uint32_t)this->viewport_size.y)) {
			this->frame_buffer->resize((uint32_t)this->viewport_size.x, (uint32_t)this->viewport_size.y);
			this->camera_controller.resize_viewport((uint32_t)this->viewport_size.x, (uint32_t)this->viewport_size.y);
		}

		if (this->viewport_focused) {
			this->camera_controller.on_update(ts);
		}

		this->frame_buffer->bind();
		elm::scene_renderer::render(scene, this->camera_controller.get_camera());
		this->frame_buffer->unbind();
	}

	void viewport_panel::on_event(event &e)
	{
		this->camera_controller.on_event(e);
	}

	void viewport_panel::on_imgui_render()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Viewport");

		this->viewport_focused = ImGui::IsWindowFocused();
		this->viewport_hovered = ImGui::IsWindowHovered();
		elm::application::get()->get_imgui_layer()->set_block_events(!this->viewport_focused || !this->viewport_hovered);

		ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
		this->viewport_size = { viewport_panel_size.x, viewport_panel_size.y };
		ImGui::Image(this->frame_buffer->get_color_attachment_renderer_id(0), { this->viewport_size.x, this->viewport_size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f });

		ImGui::End();
		ImGui::PopStyleVar();
	}
}
