#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_texture_slot;

uniform mat4 u_view_projection;

out vec2 v_uv;
out vec4 v_color;
out float v_texture_slot;

void main()
{
	v_uv = a_uv;
	v_color = a_color;
	v_texture_slot = a_texture_slot;
	gl_Position = u_view_projection * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_color;

in vec2 v_uv;
in vec4 v_color;
in float v_texture_slot;

uniform sampler2D u_textures[32];

void main()
{
	o_color = texture(u_textures[int(v_texture_slot)], v_uv) * v_color;
}
