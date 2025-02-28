#include "application.h"

namespace elm {

	Application* Application::s_instance = nullptr;

	Application::Application(void)
		: m_running(true)
	{
		// TODO: Assert on s_instance existing

		s_instance = this;
	}

	Application::~Application(void)
	{
	}

	void Application::run(void)
	{
		while (m_running) {

		}
	}
}
