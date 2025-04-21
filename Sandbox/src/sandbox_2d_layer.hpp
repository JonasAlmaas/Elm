#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct sandbox_2d_layer : elm::layer
{
	sandbox_2d_layer();
	virtual ~sandbox_2d_layer() = default;

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render() override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::orthographic_camera_controller camera_controller;

	std::shared_ptr<elm::texture_2d> texture_grass_tileset;
	std::shared_ptr<elm::texture_2d> texture_checkerboard;

	uint32_t frame_time_acc_ix = 0u;
	float frame_time_acc = 0.0f;
	float avg_frame_delta = 0.0f;

	elm::math::spline spline;
	elm::math::spline spline2;
};
