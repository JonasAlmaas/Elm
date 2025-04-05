#pragma once

#include "elm/core/base.hpp"
#include <entt/entt.hpp>

namespace elm {

	struct entity
	{
		entity(void) = default;
		entity(entt::registry *registry);
		entity(entt::registry *registry, entt::entity entity_handle);
		entity(const entity &) = default;

		template<typename T, typename... Args>
		T &add_component(Args&&... args)
		{
			ELM_CORE_ASSERT(!has_component<T>(), "Entity already has this component");
			return this->registry->emplace<T>(this->entity_handle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T &add_or_replace_componet(Args&&... args)
		{
			return this->registry->emplace_or_replace<T>(this->entity_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T &get_component(void)
		{
			ELM_CORE_ASSERT(has_component<T>(), "Entity does not have this component");
			return this->registry->get<T>(this->entity_handle);
		}

		template<typename T>
		bool has_component(void)
		{
			return this->registry->all_of<T>(this->entity_handle);
		}

		operator bool() const { return this->entity_handle != entt::null; }
		operator entt::entity() const { return this->entity_handle; }

		bool operator==(const entity &other) const { return this->entity_handle == other.entity_handle; }
		bool operator!=(const entity &other) const { return !(*this == other); }

	private:
		entt::registry *registry = nullptr;
		entt::entity entity_handle = entt::null;
	};
}
