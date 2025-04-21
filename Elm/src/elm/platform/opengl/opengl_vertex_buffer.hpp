#pragma once

#include "elm/core/renderer/vertex_buffer.hpp"
#include <stdint.h>
#include <memory>

namespace elm {

	struct opengl_vertex_buffer : vertex_buffer
	{
		opengl_vertex_buffer(uint32_t size);
		opengl_vertex_buffer(const void *vertices, uint32_t size);
		virtual ~opengl_vertex_buffer();

		virtual void bind() const override;
		virtual void unbind() const  override;

		virtual void set_data(const void *data, uint32_t size) override;

		virtual const vertex_buffer_layout *get_layout() const override {return &this->layout;}
		virtual void set_layout(const vertex_buffer_layout *layout) override;

	private:
		uint32_t renderer_id;
		vertex_buffer_layout layout;
	};
}
