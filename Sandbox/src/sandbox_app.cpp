#include "elm.h"

#include <stdio.h>

class Sandbox : public elm::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

namespace elm {

	Application *Application::Create(void)
	{
		return new Sandbox();
	}
}
