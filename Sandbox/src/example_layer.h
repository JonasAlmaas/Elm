#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

class example_layer : public elm::layer
{
public:
	example_layer(void);

	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_key_pressed(elm::key_pressed_event &e);

private:
	std::shared_ptr<elm::vertex_array> m_triangle_va;
	std::shared_ptr<elm::vertex_array> m_square_va;
	std::shared_ptr<elm::shader> m_shader;
	elm::orthographic_camera m_camera;

	float m_camera_move_speed = 1.0f;
	float m_camera_rot_speed = 10.0f;

};
