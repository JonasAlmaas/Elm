#pragma once

#include <elm.h>
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
	elm::perspective_camera_controller m_camera_controller;

	std::shared_ptr<elm::scene> m_scene;
	std::shared_ptr<elm::shader> m_pbr_shader;

	std::shared_ptr<elm::texture_2d> m_albedo_map;
	std::shared_ptr<elm::texture_2d> m_roughness_map;
	std::shared_ptr<elm::texture_2d> m_metalness_map;

	glm::vec3 m_albedo = { 1.0f, 1.0f, 1.0f };
	float m_roughness = 0.3f;
	float m_metalness = 0.0f;
};
