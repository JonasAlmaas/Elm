#pragma once

#include "layer.hpp"
#include <vector>

namespace elm {

	struct layer_stack
	{
		~layer_stack();

		void push_layer(layer *layer);
		void push_overlay(layer *layer);
		bool pop_layer(layer *layer);
		bool pop_overlay(layer *layer);

		std::vector<layer *>::iterator begin() { return this->layers.begin(); }
		std::vector<layer *>::iterator end() { return this->layers.end(); }

	private:
		std::vector<layer *> layers;
		uint32_t layer_insert_ix = 0;
	};
}
