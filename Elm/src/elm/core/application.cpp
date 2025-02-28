#include "application.h"

namespace elm {

	Application* Application::s_instance = nullptr;

	Application::Application(void)
		: m_running(true)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
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
}
