#pragma once

#include "elm/core/layer.hpp"

namespace elm {

	struct imgui_layer : layer
	{
		imgui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_event(event& e) override;

		void begin();
		void end();

		void set_block_events(bool block) {this->block_events = block;}

	private:
		void set_style();

	private:
		bool block_events = false;
	};
}
