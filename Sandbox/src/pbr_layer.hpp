#pragma once

#include <elm.hpp>
#include <memory>

struct pbr_layer : elm::layer
{
	pbr_layer(void);
	virtual ~pbr_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::perspective_camera_controller camera_controller;

	std::shared_ptr<elm::scene> scene;
	std::shared_ptr<elm::shader> pbr_shader;

	std::shared_ptr<elm::texture_2d> albedo_map;
	std::shared_ptr<elm::texture_2d> roughness_map;
	std::shared_ptr<elm::texture_2d> metalness_map;

	glm::vec3 albedo = { 1.0f, 1.0f, 1.0f };
	float roughness = 0.3f;
	float metalness = 0.0f;
};
