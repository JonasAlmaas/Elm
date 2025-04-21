#pragma once

#include "elm/core/renderer/texture.hpp"

namespace elm {

	struct opengl_texture_2d : texture_2d
	{
		opengl_texture_2d(const texture_specification &spec);
		opengl_texture_2d(const std::string &fpath, const texture_specification &spec);
		virtual ~opengl_texture_2d();

		virtual void bind(uint32_t slot = 0) override;

		virtual uint32_t get_width() const override {return this->width;}
		virtual uint32_t get_height() const override {return this->height;}

		virtual uint32_t get_renderer_id() const override {return this->renderer_id;}

		virtual void set_data(void *data, uint32_t size) override;

		virtual const texture_specification &get_spec() override {return this->spec;}

		virtual bool equal(const texture *other) const override
		{
			return this->renderer_id == ((opengl_texture_2d *)other)->renderer_id;
		}

	private:
		texture_specification spec;
		uint32_t width, height;
		std::string fpath; // Debug info

		uint32_t renderer_id;

		uint32_t/*GLenum*/ internal_format, data_format;
	};

	struct opengl_texture_cube : texture_cube
	{
		opengl_texture_cube(const texture_specification &spec);
		virtual ~opengl_texture_cube();


		virtual void bind(uint32_t slot = 0) override;

		virtual uint32_t get_width() const override {return this->width;}
		virtual uint32_t get_height() const override {return this->height;}

		virtual uint32_t get_renderer_id() const override {return this->renderer_id;}

		virtual void set_data(void *data, uint32_t size) override;

		virtual const texture_specification &get_spec() override {return this->spec;}

		virtual bool equal(const texture *other) const override
		{
			return this->renderer_id == ((opengl_texture_cube *)other)->renderer_id;
		}

	private:
		texture_specification spec;
		uint32_t width, height;

		uint32_t renderer_id;

		uint32_t/*GLenum*/ internal_format, data_format;
	};
}
