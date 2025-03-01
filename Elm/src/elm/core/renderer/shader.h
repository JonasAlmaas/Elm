#pragma once

#include "elm/core/base.h"
#include <string>

namespace elm {

	class Shader
	{
	public:
		Shader(const std::string& vertex_src, const std::string& fragment_src);
		~Shader(void);

		void bind(void) const;
		void unbind(void) const;

	private:
		uint32_t m_renderer_id;

	};
}
