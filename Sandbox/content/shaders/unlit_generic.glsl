#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

layout (std140, binding = 1) uniform model
{
	mat4 transform;
} u_model;

struct vertex_output
{
	vec2 uv;
	vec3 normal;
};

layout (location = 0) out vertex_output v_output;

void main()
{
	v_output.uv = a_uv;
	v_output.normal = a_normal;

	gl_Position = u_camera.view_projection * u_model.transform * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec2 uv;
	vec3 normal;
};

layout (location = 0) in vertex_output v_input;

layout (binding = 0) uniform sampler2D u_textures[32];

void main()
{
	// o_color = vec4((vec3(1.0, 1.0, 1.0) + v_input.normal) / 2, 1.0);
	// o_color = vec4(v_input.uv, 0.0, 1.0);

	o_color = texture(u_textures[0], v_input.uv);
}
