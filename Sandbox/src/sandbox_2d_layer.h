#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

struct sandbox_2d_layer : elm::layer
{
	sandbox_2d_layer(void);
	virtual ~sandbox_2d_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::orthographic_camera_controller m_camera_controller;

	std::shared_ptr<elm::texture_2d> m_texture_grass_tileset;
	std::shared_ptr<elm::texture_2d> m_texture_checkerboard;

	uint32_t m_frame_time_acc_ix = 0u;
	float m_frame_time_acc = 0.0f;
	float m_avg_frame_delta = 0.0f;

	elm::math::spline m_spline;
	elm::math::spline m_spline2;
};
