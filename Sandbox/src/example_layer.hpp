#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct example_layer : elm::layer
{
	example_layer(void);
	virtual ~example_layer(void) = default;

	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::orthographic_camera_controller camera_controller;

	std::shared_ptr<elm::vertex_array> triangle_va;
	std::shared_ptr<elm::vertex_array> square_va;

	elm::shader_library shader_library;
	std::shared_ptr<elm::shader> flat_color_shader;
	std::shared_ptr<elm::shader> texture_shader;
	std::shared_ptr<elm::uniform_buffer> color_uniform_buffer;

	std::shared_ptr<elm::texture_2d> texture;
	std::shared_ptr<elm::texture_2d> texture2;
};
