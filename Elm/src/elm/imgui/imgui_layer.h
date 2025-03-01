#pragma once

#include "elm/core/layer.h"

namespace elm {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void on_attach(void) override;
		virtual void on_detach(void) override;
		virtual void on_event(Event& e) override;

		void begin(void);
		void end(void);

		void set_block_events(bool block) { m_block_events = block; }

	private:
		bool m_block_events = false;

	};
}
