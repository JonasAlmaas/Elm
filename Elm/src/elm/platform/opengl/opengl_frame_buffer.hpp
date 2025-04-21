#pragma once

#include "elm/core/renderer/frame_buffer.hpp"

namespace elm {

	struct opengl_frame_buffer : frame_buffer
	{
		opengl_frame_buffer(const frame_buffer_specification &spec);
		virtual ~opengl_frame_buffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual int read_px_int(uint32_t attachment_ix, int x, int y) const override;
		virtual void clear_attachment_int(uint32_t attachment_ix, int value) override;

		virtual uint32_t get_color_attachment_renderer_id(uint32_t ix) const override
		{
			ELM_CORE_ASSERT(ix < this->color_attachments.size(), "Index out of range");
			return this->color_attachments[ix];
		}

		virtual uint32_t get_depth_attachment_renderer_id() const override
		{
			ELM_CORE_ASSERT(this->depth_attachment, "No depth attachment in frame buffer");
			return this->depth_attachment;
		}

		virtual const frame_buffer_specification *get_spec() const override {return &this->spec;}

	private:
		frame_buffer_specification spec;

		uint32_t renderer_id = 0;

		std::vector<frame_buffer_texture_specification> color_attachment_specs;
		frame_buffer_texture_specification depth_attachment_spec;

		std::vector<uint32_t> color_attachments;
		uint32_t depth_attachment = 0;
	};
}
