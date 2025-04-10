#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_color;
layout (location = 3) in int a_texture_slot;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

struct vertex_output
{
	vec2 uv;
	vec4 color;
};

layout (location = 0) out vertex_output v_output;
layout (location = 3) out flat int v_texture_slot;

void main()
{
	v_output.uv = a_uv;
	v_output.color = a_color;
	v_texture_slot = a_texture_slot;

	gl_Position = u_camera.view_projection * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec2 uv;
	vec4 color;
};

layout (location = 0) in vertex_output v_input;
layout (location = 3) in flat int v_texture_slot;

layout (binding = 0) uniform sampler2D u_textures[32];

void main()
{
	o_color = texture(u_textures[v_texture_slot], v_input.uv) * v_input.color;
}
