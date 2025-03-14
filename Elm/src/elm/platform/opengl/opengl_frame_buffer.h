#pragma once

#include "elm/core/renderer/frame_buffer.h"

namespace elm {

	struct opengl_frame_buffer : frame_buffer
	{
		opengl_frame_buffer(const struct frame_buffer_specification &spec);
		virtual ~opengl_frame_buffer(void);

		void invalidate(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual int read_px_int(uint32_t attachment_ix, int x, int y) const override;
		virtual void clear_attachment_int(uint32_t attachment_ix, int value) override;

		virtual uint32_t get_color_attachment_renderer_id(uint32_t ix) const override
		{
			ELM_CORE_ASSERT(ix < m_color_attachments.size(), "Index out of range");
			return m_color_attachments[ix];
		}

		virtual uint32_t get_depth_attachment_renderer_id(void) const override
		{
			ELM_CORE_ASSERT(m_depth_attachment, "No depth attachment in frame buffer");
			return m_depth_attachment;
		}

		inline virtual const struct frame_buffer_specification *get_spec(void) const override { return &m_spec; }

	private:
		struct frame_buffer_specification m_spec;

		uint32_t m_renderer_id = 0;

		std::vector<frame_buffer_texture_specification> m_color_attachment_specs;
		frame_buffer_texture_specification m_depth_attachment_spec;

		std::vector<uint32_t> m_color_attachments;
		uint32_t m_depth_attachment = 0;
	};
}
