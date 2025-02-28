#pragma once

#include "elm/core/application.h"

#ifdef ELM_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	elm::Log::init();

	auto app = elm::Application::Create();
	app->run();
	delete app;

	return 0;
}

#endif
