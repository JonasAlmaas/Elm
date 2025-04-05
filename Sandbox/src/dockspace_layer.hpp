#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct dockspace_layer : elm::layer
{
	dockspace_layer(void);
	virtual ~dockspace_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	elm::orthographic_camera_controller camera_controller;
	std::shared_ptr<elm::texture_2d> texture_checkerboard;

	std::shared_ptr<elm::frame_buffer> frame_buffer;

	glm::vec2 viewport_size = { 0.0f, 0.0f };
	bool viewport_focused = false;
	bool viewport_hovered = false;
};
