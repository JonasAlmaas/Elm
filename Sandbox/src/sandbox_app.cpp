#include "sandbox_app.h"
#include "example_layer.h"
#include "sandbox_2d_layer.h"
#include "sprite_sheet_layer.h"

sandbox::sandbox()
{
	//push_layer(new example_layer());
	//push_layer(new sandbox_2d_layer());
	push_layer(new sprite_sheet_layer());
}
