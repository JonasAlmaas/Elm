#include "sandbox_app.h"
#include "dockspace_layer.h"
#include "example_layer.h"
#include "sandbox_2d_layer.h"
#include "sandbox_3d_layer.h"
#include "sprite_sheet_layer.h"
#include "pbr_layer.h"

sandbox::sandbox(const struct elm::application_specification &spec, struct elm::application_command_line_args args)
	: elm::application(spec, args)
{
	//push_layer(new example_layer());
	//push_layer(new sandbox_2d_layer());
	//push_layer(new sprite_sheet_layer());
	//push_layer(new dockspace_layer());
	//push_layer(new sandbox_3d_layer());
	push_layer(new pbr_layer());
}
