#include "layer_stack.h"

namespace elm {

	LayerStack::LayerStack(void)
	{
	}

	LayerStack::~LayerStack(void)
	{
		for (auto l : m_layers) {
			delete l;
		}
	}

	void LayerStack::push_layer(Layer *layer)
	{
		m_layers.emplace(m_layers.begin() + m_layer_insert_ix, layer);
		++m_layer_insert_ix;
		layer->on_attach();
	}

	void LayerStack::push_overlay(Layer *layer)
	{
		m_layers.emplace_back(layer);
		layer->on_attach();
	}

	bool LayerStack::pop_layer(Layer *layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			--m_layer_insert_ix;
			layer->on_detach();
			delete layer;
			return true;
		} else {
			return false;
		}
	}

	bool LayerStack::pop_overlay(Layer *layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			layer->on_detach();
			delete layer;
			return true;
		} else {
			return false;
		}
	}
}
