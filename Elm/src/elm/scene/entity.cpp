#include "entity.h"

namespace elm {

	entity::entity(entt::registry *registry)
		: m_registry(registry), m_entity_handle(registry->create())
	{
	}

	entity::entity(entt::registry *registry, entt::entity entity_handle)
		: m_registry(registry), m_entity_handle(entity_handle)
	{
	}
}
