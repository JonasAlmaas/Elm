#pragma once

#include <elm.hpp>

struct sandbox_3d_layer : elm::layer
{
	sandbox_3d_layer(void);
	virtual ~sandbox_3d_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::perspective_camera_controller camera_controller;

	std::shared_ptr<elm::scene> scene;
	std::shared_ptr<elm::shader> pbr_shader;

	elm::entity dir_light;
	elm::entity point_light;
	elm::entity suzanne;

	float point_light_speed = 2.0f;
};
