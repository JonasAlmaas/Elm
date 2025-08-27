#include "sandbox_app.hpp"
#include "dockspace_layer.hpp"
#include "example_layer.hpp"
#include "sandbox_2d_layer.hpp"
#include "sandbox_3d_layer.hpp"
#include "sdf_layer.hpp"
#include "sprite_sheet_layer.hpp"
#include "pbr_layer.hpp"

sandbox::sandbox(
		const elm::application_specification &spec,
		elm::application_command_line_args args)
	: elm::application(spec, args)
{
	//push_layer(new example_layer());
	//push_layer(new sandbox_2d_layer());
	//push_layer(new sprite_sheet_layer());
	//push_layer(new dockspace_layer());
	//push_layer(new sandbox_3d_layer());
	//push_layer(new pbr_layer());
	push_layer(new sdf_layer());
}
