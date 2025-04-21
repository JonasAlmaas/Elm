#pragma once

#include "elm/core/base.hpp"
#include "entity.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	struct scene
	{
		entity create_entity() {return entity(&this->registry);}
		void destroy_entity(entity entity) {this->registry.destroy(entity.get_entity_handle());}

		void set_clear_color(const glm::vec4 &clear_color) {this->clear_color = clear_color;}
		const glm::vec4 &get_clear_color() const {return this->clear_color;}

		void set_show_world_grid(bool show) {this->show_world_grid = show;}
		bool get_show_world_grid() const {return this->show_world_grid;}

		// Should find a better solution than this
		const entt::registry &get_reg() const {return this->registry;}
		entt::registry &get_reg() {return this->registry;}

	public:
		static std::shared_ptr<scene> create() {return std::make_shared<scene>();}

	private:
		entt::registry registry;

		glm::vec4 clear_color = { 1.0f, 0.0f, 1.0f, 1.0f };
		bool show_world_grid = false;
	};
}
