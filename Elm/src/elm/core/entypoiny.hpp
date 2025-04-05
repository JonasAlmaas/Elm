#pragma once

#include "elm/core/base.hpp"
#include "elm/core/application.hpp"

#ifdef ELM_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	elm::log::init();

	ELM_PROFILE_BEGIN_SESSION("Startup", "profile-startup.json");
	auto app = elm::application::create({ argc, argv });
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
