#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

out vec2 v_uv;

void main()
{
	v_uv = a_uv;
	gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;

in vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
	o_color = texture(u_texture, v_uv);
}
