#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/vertex_buffer_element.hpp"
#include <initializer_list>
#include <stdint.h>
#include <vector>

namespace elm {

	struct vertex_buffer_layout
	{
		vertex_buffer_layout(void) = default;

		vertex_buffer_layout(const std::initializer_list<vertex_buffer_element> &elements)
			: elements(elements), stride(0)
		{
			calculate_offsets_n_stride();
		}

		inline uint32_t get_stride(void) const { return this->stride; }
		inline std::vector<vertex_buffer_element> get_elements(void) const { return this->elements; }

		std::vector<vertex_buffer_element>::iterator begin() { return this->elements.begin(); }
		std::vector<vertex_buffer_element>::iterator end() { return this->elements.end(); }
		std::vector<vertex_buffer_element>::const_iterator begin() const { return this->elements.begin(); }
		std::vector<vertex_buffer_element>::const_iterator end() const { return this->elements.end(); }

	private:
		std::vector<vertex_buffer_element> elements;
		uint32_t stride;

	private:
		void calculate_offsets_n_stride(void)
		{
			uint32_t offset = 0;
			for (auto& el : this->elements) {
				el.offset = offset;
				offset += el.size;
				this->stride += el.size;
			}
		}
	};
}
