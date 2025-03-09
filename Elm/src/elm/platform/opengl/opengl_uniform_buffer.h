#pragma once

#include "elm/core/renderer/uniform_buffer.h"

namespace elm {

	class opengl_uniform_buffer : public uniform_buffer
	{
	public:
		opengl_uniform_buffer(uint32_t size, uint32_t binding);
		~opengl_uniform_buffer(void);

		virtual void set_data(const void *data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_renderer_id;
	};
}
