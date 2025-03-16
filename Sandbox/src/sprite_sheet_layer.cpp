#include "sprite_sheet_layer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

enum {
	MAP_SIZE = 30,
};
static uint32_t s_map[MAP_SIZE * MAP_SIZE] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 24, 24, 24, 24, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 13, 13, 13, 17, 24, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0, 23, 18, 13, 13, 13, 13, 13, 13, 17, 24, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  1, 29, 13, 13, 13, 13, 13, 13, 13, 13, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2, 29, 13, 13, 13, 28,  2,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

sprite_sheet_layer::sprite_sheet_layer(void)
	: layer("Sandbox2D"), m_camera_controller(16.0f / 9.0f, false)
{
}

void sprite_sheet_layer::on_attach(void)
{
	m_texture_grass_tileset = elm::texture_2d::create("content/textures/sprout-lands/grass_tileset.png", {
		.mag_filter = elm::texture_filter::NEAREST
	});
	m_texture_water_tileset = elm::texture_2d::create("content/textures/sprout-lands/water_tileset.png", {
		.mag_filter = elm::texture_filter::NEAREST
	});

	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 11; ++x) {
			m_texture_atlas_grass_tileset[y * 11 + x] = elm::sub_texture_2d::from_atlas(m_texture_grass_tileset, 16, 16, x, y);
		}
	}

	for (int x = 0; x < 4; ++x) {
		m_texture_sprite_water[x] = elm::sub_texture_2d::from_atlas(m_texture_water_tileset, 16, 16, x, 0);
	}
}

void sprite_sheet_layer::on_detach(void)
{
}

void sprite_sheet_layer::on_update(elm::timestep ts)
{
	calculate_fps(ts);

	m_camera_controller.on_update(ts);

	static float prev_tick_time_s = 0.0f;
	float time = elm::time::get_seconds();
	if (time - prev_tick_time_s >= 1.0f / 4.0f) {
		prev_tick_time_s = time;
		tick();
	}

	elm::render_command::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
	elm::render_command::clear();

	elm::renderer_2d::reset_stats();
	elm::renderer_2d::begin_scene(m_camera_controller.get_camera());

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			uint32_t tile = s_map[y * MAP_SIZE + x];
			glm::vec2 pos = { (x - MAP_SIZE / 2) * 0.25f,  (y - MAP_SIZE / 2) * 0.25f };
			glm::vec2 size = { 0.25f, 0.25f };

			if (tile != 13) {
				elm::renderer_2d::draw_sprite(glm::vec3(pos, -0.1f), size, m_texture_sprite_water[m_water_ix]);
			}

			if (tile) {
				elm::renderer_2d::draw_sprite(pos, size, m_texture_atlas_grass_tileset[tile - 1]);
			}
		}
	}

	/*for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 11; ++x) {
			elm::renderer_2d::draw_sprite({ x, 5 - y }, { 1.0f, 1.0f }, m_texture_atlas_grass_tileset[y * 11 + x]);
		}
	}*/

	elm::renderer_2d::end_scene();
}

void sprite_sheet_layer::on_event(elm::event &e)
{
	m_camera_controller.on_event(e);

	elm::event_dispatcher dispatcher(e);

	dispatcher.dispatch<elm::window_resize_event>(ELM_BIND_EVENT_FN(sprite_sheet_layer::on_window_resize));
}

void sprite_sheet_layer::on_imgui_render(void)
{
	elm::renderer_2d::statistics stats = elm::renderer_2d::get_stats();

	ImGui::Begin("Statistics");
	ImGui::Text("FPS: %.2f", 1.0f / m_avg_frame_delta);
	ImGui::Text("Quad count: %d", stats.quad_count);
	ImGui::Text("Draw calls: %d", stats.draw_calls);

	/*uint32_t mem_usage = stats.get_memory_usage();
	if (mem_usage < 1000) {
		ImGui::Text("Memory usage: %d B", mem_usage);
	} else if (mem_usage < 1'000'000) {
		ImGui::Text("Memory usage: %.2f KB", (float)mem_usage / 1000.0f);
	} else if (mem_usage < 1'000'000'000) {
		ImGui::Text("Memory usage: %.2f MB", (float)mem_usage / 1'000'000.0f);
	} else {
		ImGui::Text("Memory usage: %.2f GB", (float)mem_usage / 1'000'000'000.0f);
	}*/

	ImGui::End();
}

void sprite_sheet_layer::calculate_fps(elm::timestep ts)
{
	static uint32_t s_frame_time_acc_ix = 0u;
	static float s_frame_time_acc = 0.0f;

	s_frame_time_acc += ts.get_seconds();
	++s_frame_time_acc_ix;
	if (s_frame_time_acc_ix >= 30) {
		m_avg_frame_delta = s_frame_time_acc / (float)s_frame_time_acc_ix;
		s_frame_time_acc = 0.0f;
		s_frame_time_acc_ix = 0;
	}
}

void sprite_sheet_layer::tick(void)
{
	if (++m_water_ix > 3) {
		m_water_ix = 0;
	}
}

bool sprite_sheet_layer::on_window_resize(elm::window_resize_event &e)
{
	m_camera_controller.resize_viewport(e.get_width(), e.get_height());
	return false;
}
