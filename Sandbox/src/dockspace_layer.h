#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

class dockspace_layer : public elm::layer
{
public:
	dockspace_layer(void);
	virtual ~dockspace_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	elm::orthographic_camera m_camera;
	std::shared_ptr<elm::texture_2d> m_texture_checkerboard;

	std::shared_ptr<elm::frame_buffer> m_frame_buffer;
};
