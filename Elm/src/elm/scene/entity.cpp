#include "entity.hpp"

namespace elm {

	entity::entity(entt::registry *registry)
		: registry(registry), entity_handle(registry->create())
	{
	}

	entity::entity(entt::registry *registry, entt::entity entity_handle)
		: registry(registry), entity_handle(entity_handle)
	{
	}
}
