#include "layer_stack.hpp"

namespace elm {

	layer_stack::~layer_stack(void)
	{
		ELM_PROFILE_FUNCTION();

		for (auto l : this->layers) {
			l->on_detach();
			delete l;
		}
	}

	void layer_stack::push_layer(layer *layer)
	{
		ELM_PROFILE_FUNCTION();

		this->layers.emplace(this->layers.begin() + this->layer_insert_ix, layer);
		++this->layer_insert_ix;
		layer->on_attach();
	}

	void layer_stack::push_overlay(layer *layer)
	{
		ELM_PROFILE_FUNCTION();

		this->layers.emplace_back(layer);
		layer->on_attach();
	}

	bool layer_stack::pop_layer(layer *layer)
	{
		ELM_PROFILE_FUNCTION();

		auto it = std::find(this->layers.begin(), this->layers.end(), layer);
		if (it != this->layers.end()) {
			this->layers.erase(it);
			--this->layer_insert_ix;
			layer->on_detach();
			delete layer;
			return true;
		} else {
			return false;
		}
	}

	bool layer_stack::pop_overlay(layer *layer)
	{
		ELM_PROFILE_FUNCTION();

		auto it = std::find(this->layers.begin(), this->layers.end(), layer);
		if (it != this->layers.end()) {
			this->layers.erase(it);
			layer->on_detach();
			delete layer;
			return true;
		} else {
			return false;
		}
	}
}
