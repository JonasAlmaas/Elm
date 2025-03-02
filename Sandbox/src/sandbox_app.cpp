#include "sandbox_app.h"
#include "example_layer.h"
#include "sandbox_2d_layer.h"

sandbox::sandbox()
{
	//push_layer(new example_layer());
	push_layer(new sandbox_2d_layer());
}
