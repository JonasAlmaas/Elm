#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

struct vertex_output
{
	vec3 frag_pos;
};

layout (location = 0) out vertex_output v_output;

void main()
{
	v_output.frag_pos = a_position;
	gl_Position = u_camera.view_projection * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec3 frag_pos;
};

layout (location = 0) in vertex_output v_input;
layout (binding = 0) uniform sampler2D u_environment_map;

const vec2 inv_atan = vec2(0.1591, 0.3183);

vec2 sample_spherical_map(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= inv_atan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = sample_spherical_map(normalize(v_input.frag_pos));
	vec3 color = texture(u_environment_map, uv).rgb;

	o_color = vec4(color, 1.0);
}
