#include "sdf_layer.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

sdf_layer::sdf_layer()
	: layer("Dockspace layer"), camera_controller(16.0f / 9.0f, false)
{
}

void sdf_layer::on_attach()
{
	this->sdf_params_ub = elm::uniform_buffer::create(sizeof this->sdf_params_ub_data, 1);
	this->sdf_shader = elm::shader::create("content/shaders/sdf.glsl");

	this->sdf_params_ub_data.time = elm::time::get_seconds();
	this->sdf_params_ub_data.threshold = 0.0f;
	this->sdf_params_ub_data.smoothing = 2.5f;
	this->sdf_params_ub_data.debug_distance = false;

	this->frame_buffer = elm::frame_buffer::create({
		.width = 1280,
		.height = 720,
		.attachments = {
			elm::frame_buffer_texture_format::RGBA8,
			elm::frame_buffer_texture_format::DEPTH24STENCIL8
		}
		});
}

void sdf_layer::on_detach()
{
}

void sdf_layer::on_update(elm::timestep ts)
{
	if (auto spec = this->frame_buffer->get_spec();
		this->viewport_size.x > 0.0f && this->viewport_size.y > 0.0f
		&& (spec->width != (uint32_t)this->viewport_size.x || spec->height != (uint32_t)this->viewport_size.y)) {
		this->frame_buffer->resize((uint32_t)this->viewport_size.x, (uint32_t)this->viewport_size.y);
		this->camera_controller.resize_viewport((uint32_t)this->viewport_size.x, (uint32_t)this->viewport_size.y);
	}

	if (this->viewport_focused) {
		this->camera_controller.on_update(ts);
	}

	this->frame_buffer->bind();

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	//elm::renderer_2d::begin_scene(this->camera_controller.get_camera());

	//elm::renderer_2d::draw_sprite({ -0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//elm::renderer_2d::draw_sprite({ 0.5f, 0.5f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });

	//elm::renderer_2d::end_scene();

	this->sdf_params_ub_data.time = elm::time::get_seconds();
	this->sdf_params_ub->set_data(&this->sdf_params_ub_data, sizeof this->sdf_params_ub_data);
	this->sdf_shader->bind();
	elm::render_command::draw_triangles(6);

	this->frame_buffer->unbind();
}

void sdf_layer::on_event(elm::event &e)
{
	this->camera_controller.on_event(e);
}

void sdf_layer::on_imgui_render()
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

	// -- Settings --
	ImGui::Begin("Settings");

	if (ImGui::Button("Refresh SDF Shader")) this->sdf_shader->reload();
	ImGui::DragFloat("Threshold", &this->sdf_params_ub_data.threshold, 0.1f, -20.0f, 20.0f);
	ImGui::DragFloat("Smoothing", &this->sdf_params_ub_data.smoothing, 0.1f, 0.0f, 20.0f);
	bool use_debug_distance = this->sdf_params_ub_data.debug_distance;
	if (ImGui::Checkbox("Debug distance", &use_debug_distance)) {
		this->sdf_params_ub_data.debug_distance = use_debug_distance ? 1 : 0;
	}

	ImGui::End();

	// -- Viewport --
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
