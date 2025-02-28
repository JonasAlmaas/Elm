#pragma once

#include "layer.h"
#include <vector>

namespace elm {

	class LayerStack
	{
	public:
		LayerStack(void);
		~LayerStack(void);

		void push_layer(Layer *layer);
		void push_overlay(Layer *layer);
		bool pop_layer(Layer *layer);
		bool pop_overlay(Layer *layer);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		uint32_t m_layer_insert_ix;

	};
}
