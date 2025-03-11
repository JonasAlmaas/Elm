#pragma once

#include "elm/core/base.h"
#include "entity.h"
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	class scene
	{
	public:
		entity create_entity(void);
		void destroy_entity(entity entity);

		inline void set_clear_color(const glm::vec4 &clear_color) { m_clear_color = clear_color; }
		inline const glm::vec4 &get_clear_color(void) const { return m_clear_color; }

	public:
		static std::shared_ptr<scene> create(void);

	private:
		entt::registry m_registry;

		glm::vec4 m_clear_color = { 1.0f, 0.0f, 1.0f, 1.0f };
	};
}
