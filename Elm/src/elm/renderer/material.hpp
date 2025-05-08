#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/shader.hpp"
#include "elm/core/renderer/texture.hpp"
#include <memory>

namespace elm {

	struct material
	{
		virtual ~material() = default;

		virtual void bind() = 0;

	public:
		std::shared_ptr<shader> shader;
	};

	struct pbr_material final : material
	{
		virtual ~pbr_material() = default;

		virtual void bind() override
		{
			//shader->bind();

			albedo->bind(3);
			normal->bind(4);
			roughness->bind(5);
			ao->bind(6);
			metalness->bind(7);
			//transparancy->bind(8);
		}

	public:
		std::shared_ptr<texture_2d> albedo;
		std::shared_ptr<texture_2d> normal;
		std::shared_ptr<texture_2d> roughness;
		std::shared_ptr<texture_2d> ao;
		std::shared_ptr<texture_2d> metalness;
		//std::shared_ptr<texture_2d> transparancy; // TODO: Maybe
	};
}
