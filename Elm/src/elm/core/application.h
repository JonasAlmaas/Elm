#pragma once

#include "elm/core/base.h"
#include "elm/core/event/event.h"
#include "elm/core/window.h"
#include <memory>

namespace elm {

	class Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		// To be defined by the client
		static Application *Create(void);

		void run(void);

		void on_event(Event& e);

	public:
		static inline Application *get(void) { return s_instance; }

	private:
		std::unique_ptr<Window> m_window;
		bool m_running;

	private:
		static Application *s_instance;
	};
}
