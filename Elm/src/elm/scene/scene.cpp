#include "scene.hpp"

namespace elm {

	std::shared_ptr<scene> scene::create(void)
	{
		return std::make_shared<scene>();
	}

	entity scene::create_entity(void)
	{
		return entity(&this->registry);
	}

	void scene::destroy_entity(entity entity)
	{
		this->registry.destroy(entity);
	}
}
