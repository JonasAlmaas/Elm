#pragma once

#include "elm/core/base.h"
#include "entity.h"
#include <entt/entt.hpp>
#include <memory>

namespace elm {

	class scene
	{
	public:
		entity create_entity(void);
		void destroy_entity(entity entity);

	public:
		static std::shared_ptr<scene> create(void);

	private:
		entt::registry m_registry;
	};
}
