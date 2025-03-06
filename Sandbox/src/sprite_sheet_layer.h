#pragma once

#include <elm.h>
#include <glm/glm.hpp>
#include <memory>

class sprite_sheet_layer : public elm::layer
{
public:
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
	elm::orthographic_camera_controller m_camera_controller;

	float m_avg_frame_delta = 0.0f;
	uint32_t m_water_ix = 0;

	std::shared_ptr<elm::texture_2d> m_texture_grass_tileset;
	std::shared_ptr<elm::texture_2d> m_texture_water_tileset;

	std::array<std::shared_ptr<elm::sub_texture_2d>, 55> m_texture_atlas_grass_tileset;
	std::array<std::shared_ptr<elm::sub_texture_2d>, 4> m_texture_sprite_water;
};
