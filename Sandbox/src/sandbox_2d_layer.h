#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

class sandbox_2d_layer : public elm::layer
{
public:
	sandbox_2d_layer(void);
	virtual ~sandbox_2d_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	elm::orthographic_camera_controller m_camera_controller;

};
