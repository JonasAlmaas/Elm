#type vertex
#version 450 core

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
	vec3 position;
} u_camera;

struct vertex_output
{
	vec2 world_pos;
	vec2 camera_pos;
	float grid_size;
};

layout (location = 0) out vertex_output v_output;

const vec3 vertices[4] = vec3[4](
	vec3(-0.5, -0.5, 0.0),
	vec3( 0.5, -0.5, 0.0),
	vec3( 0.5,  0.5, 0.0),
	vec3(-0.5,  0.5, 0.0)
);

const int indices[6] = int[6](0, 1, 2, 0, 2, 3);

void main()
{
	float grid_size = 200.0; // TODO: Take in from a uniform buffer

	vec3 pos = vertices[indices[gl_VertexIndex]];
	pos *= grid_size;
	pos.xy += u_camera.position.xy;

	v_output.world_pos = pos.xy;
	v_output.camera_pos = u_camera.position.xy;
	v_output.grid_size = grid_size;

	gl_Position = u_camera.view_projection * vec4(pos, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec2 world_pos;
	vec2 camera_pos;
	float grid_size;
};

layout (location = 0) in vertex_output v_input;

const float grid_min_px_between_cells = 2.0;
const float grid_cell_size = 0.1;
const vec4 grid_color_thin = vec4(0.3, 0.3, 0.3, 1.0);
const vec4 grid_color_thick = vec4(0.5, 0.5, 0.5, 1.0);

float log10(float x)
{
	float f = log(x) / log(10.0);
	return f;
}

float max2(vec2 v)
{
	return max(v.x, v.y);
}

void main()
{
	vec2 dvx = vec2(dFdx(v_input.world_pos.x), dFdy(v_input.world_pos.x));
	vec2 dvy = vec2(dFdx(v_input.world_pos.y), dFdy(v_input.world_pos.y));

	float lx = length(dvx);
	float ly = length(dvy);

	vec2 dudv = vec2(lx, ly);

	float l = length(dudv);

	float lod = max(0.0, log10(l * grid_min_px_between_cells / grid_cell_size) + 1.0);

	float grid_cell_size_lod_0 = grid_cell_size * pow(10.0, floor(lod));
	float grid_cell_size_lod_1 = grid_cell_size_lod_0 * 10.0;
	float grid_cell_size_lod_2 = grid_cell_size_lod_1 * 10.0;

	dudv *= 4.0;

	vec2 mod_div_dudv = mod(v_input.world_pos, grid_cell_size_lod_0) / dudv;
	float lod_0a = max2(vec2(1.0) - abs(clamp(mod_div_dudv, 0.0, 1.0) * 2.0 - vec2(1.0)));

	mod_div_dudv = mod(v_input.world_pos, grid_cell_size_lod_1) / dudv;
	float lod_1a = max2(vec2(1.0) - abs(clamp(mod_div_dudv, 0.0, 1.0) * 2.0 - vec2(1.0)));

	mod_div_dudv = mod(v_input.world_pos, grid_cell_size_lod_2) / dudv;
	float lod_2a = max2(vec2(1.0) - abs(clamp(mod_div_dudv, 0.0, 1.0) * 2.0 - vec2(1.0)));

	float lod_fade = fract(lod);

	vec4 color;

	if (lod_2a > 0.0) {
		float absx = abs(v_input.world_pos.x);
		float absy = abs(v_input.world_pos.y);

		if (absy < grid_cell_size && absx >= absy) {
			color = vec4(0.8, 0.2, 0.1, 1.0);
		} else if (absx < grid_cell_size && absy >= absx) {
			color = vec4(0.2, 0.8, 0.3, 1.0);
		} else {
			color = grid_color_thick;
		}

		color.a *= lod_2a;
	} else {
		if (lod_1a > 0.0) {
			color = mix(grid_color_thick, grid_color_thin, lod_fade);
			color.a *= lod_1a;
		} else {
			color = grid_color_thin;
			color.a *= (lod_0a * (1.0 - lod_fade));
		}
	}

	float opacity_falloff = 1.0 - clamp(length(v_input.world_pos - v_input.camera_pos) / v_input.grid_size, 0.0, 1.0);
	color.a *= opacity_falloff;

	o_color = color;
}
