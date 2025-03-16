#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;
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

float screen_px_range() {
	const float px_range = 2.0; // set to distance field's pixel range
	vec2 unit_range = vec2(px_range) / vec2(textureSize(u_textures[v_texture_slot], 0));
	vec2 screen_tex_size = vec2(1.0) / fwidth(v_input.uv);
	return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec3 msd = texture(u_textures[v_texture_slot], v_input.uv).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screen_px_distance = screen_px_range() * (sd - 0.5);
	float opacity = clamp(screen_px_distance + 0.5, 0.0, 1.0);
	if (opacity == 0.0) {
		discard;
	}

	vec4 bg_color = vec4(0.0);
	o_color = mix(bg_color, v_input.color, opacity);
	if (o_color.a == 0.0) {
		discard;
	}
}
