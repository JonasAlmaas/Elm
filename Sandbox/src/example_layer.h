#pragma once

#include <elm.h>
#include <memory>

class example_layer : public elm::layer
{
public:
	example_layer(void);

	virtual void on_update(void) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_key_pressed(elm::key_pressed_event &e);

private:
	std::shared_ptr<elm::vertex_array> m_vertex_array;
	std::shared_ptr<elm::vertex_array> m_vertex_array2;
	std::shared_ptr<elm::shader> m_shader;
	elm::orthographic_camera m_camera;

	float m_camera_move_speed = 0.01f;
	float m_camera_rot_speed = 0.1f;

};
