#include "scene.h"

namespace elm {

	std::shared_ptr<scene> scene::create(void)
	{
		return std::make_shared<scene>();
	}

	entity scene::create_entity(void)
	{
		return entity(&m_registry);
	}

	void scene::destroy_entity(entity entity)
	{
		m_registry.destroy(entity);
	}
}
