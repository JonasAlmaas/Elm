#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

struct sprite_sheet_layer : elm::layer
{
	sprite_sheet_layer(void);
	virtual ~sprite_sheet_layer(void) = default;

	virtual void on_attach(void) override;
	virtual void on_detach(void) override;
	virtual void on_update(elm::timestep ts) override;
	virtual void on_event(elm::event &e) override;
	virtual void on_imgui_render(void) override;

private:
	void calculate_fps(elm::timestep ts);
	void tick(void);

private:
	bool on_window_resize(elm::window_resize_event &e);

private:
	elm::orthographic_camera_controller camera_controller;

	float avg_frame_delta = 0.0f;
	uint32_t animation_ix = 0;

	std::shared_ptr<elm::texture_2d> texture_grass_tileset;
	std::shared_ptr<elm::texture_2d> texture_water_tileset;

	std::array<std::shared_ptr<elm::sub_texture_2d>, 55> texture_atlas_grass_tileset;
	std::array<std::shared_ptr<elm::sub_texture_2d>, 4> texture_sprite_water;
};
