#pragma once

#include <elm.h>

class sandbox_3d_layer : public elm::layer
{
public:
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
	elm::perspective_camera_controller m_camera_controller;

	std::shared_ptr<elm::scene> m_scene;

	std::shared_ptr<elm::shader> m_shader;
	std::shared_ptr<elm::vertex_array> m_vertex_array;
	std::shared_ptr<elm::texture_2d> m_texture_checkerboard;
};
