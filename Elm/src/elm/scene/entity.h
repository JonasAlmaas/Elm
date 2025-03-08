#pragma once

#include "elm/core/base.h"
#include <entt/entt.hpp>

namespace elm {

	class entity
	{
	public:
		entity(void) = default;
		entity(entt::registry *registry);
		entity(entt::registry *registry, entt::entity entity_handle);
		entity(const entity &) = default;

		template<typename T, typename... Args>
		T &add_component(Args&&... args)
		{
			ELM_CORE_ASSERT(!has_component<T>(), "Entity already has this component");
			return m_registry->emplace<T>(m_entity_handle, std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		T &add_or_replace_componet(Args&&... args)
		{
			return m_registry->emplace_or_replace<T>(m_entity_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T &get_component(void)
		{
			ELM_CORE_ASSERT(has_component<T>(), "Entity does not have this component");
			return m_registry->get<T>(m_entity_handle);
		}

		template<typename T>
		bool has_component(void)
		{
			return m_registry->all_of<T>(m_entity_handle);
		}

	public:
		operator bool() const { return m_entity_handle != entt::null; }
		operator entt::entity() const { return m_entity_handle; }

		bool operator==(const entity &other) const { return m_entity_handle == other.m_entity_handle; }
		bool operator!=(const entity &other) const { return !(*this == other); }

	private:
		entt::registry *m_registry = nullptr;
		entt::entity m_entity_handle = entt::null;
	};
}
