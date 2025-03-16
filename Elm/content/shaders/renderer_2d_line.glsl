#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

struct vertex_output
{
	vec4 color;
};

layout (location = 0) out vertex_output v_output;

void main()
{
	v_output.color = a_color;

	gl_Position = u_camera.view_projection * vec4(a_position, 1.0);
}

#type pixel
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec4 color;
};

layout (location = 0) in vertex_output v_input;

void main()
{
	o_color = v_input.color;
}
