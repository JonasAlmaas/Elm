#include "layer_stack.h"

namespace elm {

	layer_stack::layer_stack(void)
	{
	}

	layer_stack::~layer_stack(void)
	{
		for (auto l : m_layers) {
			delete l;
		}
	}

	void layer_stack::push_layer(layer *layer)
	{
		m_layers.emplace(m_layers.begin() + m_layer_insert_ix, layer);
		++m_layer_insert_ix;
		layer->on_attach();
	}

	void layer_stack::push_overlay(layer *layer)
	{
		m_layers.emplace_back(layer);
		layer->on_attach();
	}

	bool layer_stack::pop_layer(layer *layer)
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

	bool layer_stack::pop_overlay(layer *layer)
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
