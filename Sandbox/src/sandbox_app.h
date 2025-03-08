#pragma once

#include <elm.h>

class sandbox : public elm::application
{
public:
	sandbox(const struct elm::application_specification &spec);
};
