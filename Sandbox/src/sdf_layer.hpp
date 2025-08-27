#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct sdf_params_ub_s {
	float time;
	float threshold;
	float smoothing;
	int debug_distance; // boolean value
};

struct sdf_layer final : elm::layer
{
	sdf_layer();
	virtual ~sdf_layer() = default;

	virtual void on_attach() override;
	virtual void on_detach() override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render() override;

private:
	elm::orthographic_camera_controller camera_controller;

	std::shared_ptr<elm::frame_buffer> frame_buffer;

	std::shared_ptr<elm::uniform_buffer> sdf_params_ub;
	sdf_params_ub_s sdf_params_ub_data;

	std::shared_ptr<elm::shader> sdf_shader;

	glm::vec2 viewport_size = { 0.0f, 0.0f };
	bool viewport_focused = false;
	bool viewport_hovered = false;
};
