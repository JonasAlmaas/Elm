#include "application.h"

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace elm {

	Application* Application::s_instance = nullptr;

	Application::Application(void)
		: m_running(true)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(Application::on_event));
	}

	Application::~Application(void)
	{
	}

	void Application::run(void)
	{
		while (m_running) {
			m_window->on_update();
		}
	}

	void Application::on_event(Event& e)
	{
		ELM_CORE_TRACE("{0}", e.to_string());
		/*EventDispatcher dispatcher(e); */
	}
}
