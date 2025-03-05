#pragma once

#include "elm/core/base.h"
#include <memory>
#include <stdint.h>
#include <string>

namespace elm {

	class texture
	{
	public:
		virtual ~texture(void) = default;

		virtual void bind(uint32_t slot = 0) = 0;

		virtual void set_data(void *data, uint32_t size) = 0;

		virtual uint32_t get_width(void) const = 0;
		virtual uint32_t get_height(void) const = 0;

		virtual bool equal(const texture *other) const = 0;
	};

	class texture_2d : public texture
	{
	public:
		virtual ~texture_2d(void) = default;

	public:
		static std::shared_ptr<texture_2d> create(uint32_t width, uint32_t height);
		static std::shared_ptr<texture_2d> create(const std::string &fpath);
	};
}
