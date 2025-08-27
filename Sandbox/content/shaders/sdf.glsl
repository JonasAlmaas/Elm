#type vertex
#version 450 core

struct vertex_output
{
	vec2 uv;
};

layout (location = 0) out vertex_output v_output;

const vec3 vertices[4] = vec3[4](
	vec3(-1.0, -1.0, 0.0),
	vec3( 1.0, -1.0, 0.0),
	vec3( 1.0,  1.0, 0.0),
	vec3(-1.0,  1.0, 0.0)
);
const vec2 uvs[4] = vec2[4](
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(1.0, 1.0),
	vec2(0.0, 1.0)
);
const int indices[6] = int[6](0, 1, 2, 0, 2, 3);

void main()
{
	v_output.uv = uvs[indices[gl_VertexIndex]];
	gl_Position = vec4(vertices[indices[gl_VertexIndex]].xyz, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec2 uv;
};

layout (location = 0) in vertex_output v_input;

layout (std140, binding = 1) uniform params
{
	float time;
	float threshold;
	float smoothing;
	int debug_distance;
} u_params;

// https://iquilezles.org/articles/distfunctions2d

float sd_circle(vec2 p, float r)
{
	return length(p) - r;
}

float sd_box(vec2 p, vec2 b)
{
	vec2 d = abs(p) - b;
	return length(max(d, 0.0)) + min(max(d.x,d.y), 0.0);
}

float smin(float a, float b, float k)
{
	k *= 1.0/(1.0-sqrt(0.5));
	return max(k,min(a,b))
		- length(max(k-vec2(a,b),0.0));
}

float sdf(vec2 a, float smoothing, float t)
{
	// float d1 = sd_circle(a, 20);
	// float d2 = sd_box(a, vec2(10, 40));

	float d1 = sd_circle(a + vec2(sin(t) * 40, cos(t) * 40), 20);
	float d2 = sd_box(a + vec2(sin(t*0.5) * 10, cos(t*2) * 4), vec2(10, 40));

	return smin(d1, d2, smoothing);
}

// Positive colors
const vec3 color_pa = vec3(0.93, 0.56, 0.23);
const vec3 color_pb = vec3(0.79, 0.41, 0.19);

// Negative colors
const vec3 color_na = vec3(0.2, 0.35, 0.75);
const vec3 color_nb = vec3(0.18, 0.53, 0.8);

void main()
{
	float d = sdf(
		(v_input.uv + vec2(-0.5, -0.5)) * 200.0,
		u_params.smoothing,
		u_params.time);
	float d_sin = sin(d /*+ u_params.time*/);

	float sin_a = (d_sin * 0.5) + 0.5;
	float sin_b = ((d_sin * -1) * 0.5) + 0.5;

	vec3 color;
	if (u_params.debug_distance==1) {
		color = d > u_params.threshold
			? color_pa * step(0.5, sin_a) + color_pb * step(0.5, sin_b)
			: color_na * step(0.5, sin_a) + color_nb * step(0.5, sin_b);
	} else {
		color = d > u_params.threshold
			? vec3(0.0, 0.0, 0.0)
			: vec3(1.0, 1.0, 1.0);
	}

	o_color = vec4(color, 1.0);
}
