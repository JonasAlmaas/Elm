#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/vertex_buffer_element.h"
#include <initializer_list>
#include <stdint.h>
#include <vector>

namespace elm {

	class vertex_buffer_layout
	{
	public:
		vertex_buffer_layout(void) = default;

		vertex_buffer_layout(const std::initializer_list<vertex_buffer_element> &elements)
			: m_elements(elements), m_stride(0)
		{
			calculate_offsets_n_stride();
		}

		inline uint32_t get_stride(void) const { return m_stride; }
		inline std::vector<vertex_buffer_element> get_elements(void) const { return m_elements; }

		std::vector<vertex_buffer_element>::iterator begin() { return m_elements.begin(); }
		std::vector<vertex_buffer_element>::iterator end() { return m_elements.end(); }
		std::vector<vertex_buffer_element>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<vertex_buffer_element>::const_iterator end() const { return m_elements.end(); }

	private:
		std::vector<vertex_buffer_element> m_elements;
		uint32_t m_stride;

	private:
		void calculate_offsets_n_stride(void)
		{
			uint32_t offset = 0;
			for (auto& el : m_elements) {
				el.offset = offset;
				offset += el.size;
				m_stride += el.size;
			}
		}
	};
}
