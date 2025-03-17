#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/texture.h"
#include <memory>

namespace elm {

	struct material
	{
		virtual ~material(void) = default;

		virtual void bind(void) = 0;

	public:
		std::shared_ptr<shader> shader;
	};

	struct pbr_material : material
	{
		virtual ~pbr_material(void) = default;

		virtual void bind(void) override
		{
			//shader->bind();

			albedo->bind(3);
			normal->bind(4);
			rougness->bind(5);
			ao->bind(6);
			metalness->bind(7);
			//transparancy->bind(8);
		}

	public:
		std::shared_ptr<texture_2d> albedo;
		std::shared_ptr<texture_2d> normal;
		std::shared_ptr<texture_2d> rougness;
		std::shared_ptr<texture_2d> ao;
		std::shared_ptr<texture_2d> metalness;
		//std::shared_ptr<texture_2d> transparancy; // TODO: Maybe
	};
}
