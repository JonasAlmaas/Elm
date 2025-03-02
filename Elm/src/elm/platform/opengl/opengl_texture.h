#pragma once

#include "elm/core/renderer/texture.h"

namespace elm {

	class opengl_texture_2d : public texture_2d
	{
	public:
		opengl_texture_2d(const std::string &fpath);
		virtual ~opengl_texture_2d(void);

		virtual void bind(uint32_t slot = 0) override;

		inline virtual uint32_t get_width(void) const override { return m_width; }
		inline virtual uint32_t get_height(void) const override { return m_height; }

	private:
		uint32_t m_width, m_height;
		std::string m_fpath;

		uint32_t m_renderer_id;
	};
}
