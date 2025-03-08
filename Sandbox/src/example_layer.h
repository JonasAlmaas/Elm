#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

class example_layer : public elm::layer
{
public:
	example_layer(void);
	virtual ~example_layer(void) = default;

	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::orthographic_camera_controller m_camera_controller;

	std::shared_ptr<elm::vertex_array> m_triangle_va;
	std::shared_ptr<elm::vertex_array> m_square_va;

	elm::shader_library m_shader_library;
	std::shared_ptr<elm::shader> m_flat_color_shader;
	std::shared_ptr<elm::shader> m_texture_shader;

	std::shared_ptr<elm::texture_2d> m_texture;
	std::shared_ptr<elm::texture_2d> m_texture2;
};
