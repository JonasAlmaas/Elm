#pragma once

#include "elm/core/application.h"

#ifdef ELM_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	elm::log::init();

	auto app = elm::application::create();
	app->run();
	delete app;

	return 0;
}

#endif
