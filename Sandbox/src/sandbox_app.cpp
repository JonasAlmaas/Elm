#include "sandbox_app.h"
#include "example_layer.h"

sandbox::sandbox()
{
	push_layer(new example_layer());
}
