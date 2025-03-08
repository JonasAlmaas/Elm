#pragma once

#include "elm/core/renderer/frame_buffer.h"

namespace elm {

	class opengl_frame_buffer : public frame_buffer
	{
	public:
		opengl_frame_buffer(const struct frame_buffer_specification &spec);
		virtual ~opengl_frame_buffer(void);

		void invalidate(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t get_color_attachment_renderer_id(void) const override { return m_color_attachment; }

		inline virtual const struct frame_buffer_specification *get_spec(void) const override { return &m_spec; }

	private:
		struct frame_buffer_specification m_spec;

		uint32_t m_renderer_id = 0;

		uint32_t m_color_attachment = 0;
		uint32_t m_depth_attachment = 0;
	};
}
