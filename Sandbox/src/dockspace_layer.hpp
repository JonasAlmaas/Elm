#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct dockspace_layer final : elm::layer
{
	dockspace_layer();
	virtual ~dockspace_layer() = default;

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render() override;

private:
	elm::orthographic_camera_controller camera_controller;
	std::shared_ptr<elm::texture_2d> texture_checkerboard;

	std::shared_ptr<elm::frame_buffer> frame_buffer;

	glm::vec2 viewport_size = { 0.0f, 0.0f };
	bool viewport_focused = false;
	bool viewport_hovered = false;
};
