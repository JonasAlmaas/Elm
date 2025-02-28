#include <elm.h>
#include <elm/core/entypoiny.h>

#include "sandbox_app.h"

namespace elm {

	Application* Application::Create(void)
	{
		return new Sandbox();
	}
}
