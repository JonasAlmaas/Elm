#pragma once

#include "elm/core/layer.h"

namespace elm {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void on_attach(void) override;
		void on_detach(void) override;
		void on_update(void) override;
		void on_event(Event& e) override;

		void set_block_events(bool block) { m_block_events = block; }

	private:
		float m_time = 0.0f;
		bool m_block_events = false;

	};
}
