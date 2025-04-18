#pragma once

#include "elm/core/layer.hpp"

namespace elm {

	struct imgui_layer : layer
	{
		imgui_layer(void);

		virtual void on_attach(void) override;
		virtual void on_detach(void) override;
		virtual void on_event(event& e) override;

		void begin(void);
		void end(void);

		void set_block_events(bool block) { this->block_events = block; }

	private:
		void set_style(void);

	private:
		bool block_events = false;
	};
}
