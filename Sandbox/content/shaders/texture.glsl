#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

layout (std140, binding = 1) uniform model
{
	mat4 transform;
} u_model;

layout (location = 0) out vec2 v_uv;

void main()
{
	v_uv = a_uv;

	gl_Position = u_camera.view_projection * u_model.transform * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

layout (location = 0) in vec2 v_uv;

layout (binding = 0) uniform sampler2D u_texture;

void main()
{
	o_color = texture(u_texture, v_uv);
}
