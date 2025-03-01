#pragma once

#include "layer.h"
#include <vector>

namespace elm {

	class layer_stack
	{
	public:
		layer_stack(void);
		~layer_stack(void);

		void push_layer(layer *layer);
		void push_overlay(layer *layer);
		bool pop_layer(layer *layer);
		bool pop_overlay(layer *layer);

		std::vector<layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<layer*> m_layers;
		uint32_t m_layer_insert_ix = 0;

	};
}
