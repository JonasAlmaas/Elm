#pragma once

#include "elm/core/base.h"
#include "elm/core/application.h"

#ifdef ELM_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	elm::log::init();

	ELM_PROFILE_BEGIN_SESSION("Startup", "profile-startup.json");
	auto app = elm::application::create();
	ELM_PROFILE_END_SESSION();

	ELM_PROFILE_BEGIN_SESSION("Runtime", "profile-runtime.json");
	app->run();
	ELM_PROFILE_END_SESSION();

	ELM_PROFILE_BEGIN_SESSION("Shutdown", "profile-shutdown.json");
	delete app;
	ELM_PROFILE_END_SESSION();

	return 0;
}

#endif
