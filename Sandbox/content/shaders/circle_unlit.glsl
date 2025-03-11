#type vertex
#version 450 core

layout (location = 0) in vec3 a_world_position;
layout (location = 1) in vec3 a_local_position;
layout (location = 2) in vec4 a_color;
layout (location = 3) in float a_thickness;
layout (location = 4) in float a_fade;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

struct vertex_output
{
	vec3 local_pos;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out vertex_output v_output;

void main()
{
	v_output.local_pos = a_local_position;
	v_output.color = a_color;
	v_output.thickness = a_thickness;
	v_output.fade = a_fade;

	gl_Position = u_camera.view_projection * vec4(a_world_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec3 local_pos;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in vertex_output v_input;

void main()
{
	float distance = 1.0 - length(v_input.local_pos);
	float alpha = smoothstep(0.0, v_input.fade, distance);
	alpha *= smoothstep(v_input.thickness + v_input.fade, v_input.thickness, distance);

	o_color = v_input.color;
	o_color.a *= alpha;

	if (alpha == 0.0) {
		discard;
	}
}
