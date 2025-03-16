#pragma once

#include "elm/core/renderer/texture.h"

namespace elm {

	struct opengl_texture_2d : texture_2d
	{
		opengl_texture_2d(const texture_specification &spec);
		opengl_texture_2d(const std::string &fpath, const texture_specification &spec);
		virtual ~opengl_texture_2d(void);

		virtual void bind(uint32_t slot = 0) override;

		inline virtual uint32_t get_width(void) const override { return m_width; }
		inline virtual uint32_t get_height(void) const override { return m_height; }

		inline virtual uint32_t get_renderer_id(void) const override { return m_renderer_id; }

		virtual void set_data(void *data, uint32_t size) override;

		inline virtual const texture_specification &get_spec(void) override { return m_spec; }

		inline virtual bool equal(const texture *other) const override {
			return m_renderer_id == ((opengl_texture_2d *)other)->m_renderer_id;
		}

	private:
		texture_specification m_spec;
		uint32_t m_width, m_height;
		std::string m_fpath; // Debug info

		uint32_t m_renderer_id;

		uint32_t/*GLenum*/ m_internal_format, m_data_format;
	};

	struct opengl_texture_cube : texture_cube
	{
		opengl_texture_cube(const texture_specification &spec);
		virtual ~opengl_texture_cube(void);


		virtual void bind(uint32_t slot = 0) override;

		inline virtual uint32_t get_width(void) const override { return m_width; }
		inline virtual uint32_t get_height(void) const override { return m_height; }

		inline virtual uint32_t get_renderer_id(void) const override { return m_renderer_id; }

		virtual void set_data(void *data, uint32_t size) override;

		inline virtual const texture_specification &get_spec(void) override { return m_spec; }

		inline virtual bool equal(const texture *other) const override {
			return m_renderer_id == ((opengl_texture_cube *)other)->m_renderer_id;
		}

	private:
		texture_specification m_spec;
		uint32_t m_width, m_height;

		uint32_t m_renderer_id;

		uint32_t/*GLenum*/ m_internal_format, m_data_format;
	};
}
