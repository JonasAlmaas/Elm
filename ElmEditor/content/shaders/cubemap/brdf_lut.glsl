#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;

struct vertex_output
{
	vec2 uv;
};

layout (location = 0) out vertex_output v_output;

void main()
{
	v_output.uv = a_uv;
	gl_Position = vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec2 o_color;

struct vertex_output
{
	vec2 uv;
};

layout (location = 0) in vertex_output v_input;

const float PI = 3.14159265359;

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float radical_inverse_vdc(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint sample_count)
{
	return vec2(float(i) / float(sample_count), radical_inverse_vdc(i));
}

vec3 importance_sample_ggx(uint i, uint sample_count, vec3 n, float roughness)
{
	vec2 xi = hammersley(i, sample_count);

	float a = roughness*roughness;

	float phi = 2.0 * PI * xi.x;
	float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (a*a - 1.0) * xi.y));
	float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	// From spherical coordinates to tangent space cartesian coordinates
	vec3 h = vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
	vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, n));
	vec3 bitangent = cross(n, tangent);
	vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;
	return normalize(sample_vec);
}

float geometry_schlick_ggx(float n_dot_v, float roughness)
{
	// Note that we use a different k for IBL
	float a = roughness;
	float k = (a * a) / 2.0;

	float nom = n_dot_v;
	float denom = n_dot_v * (1.0 - k) + k;

	return nom / denom;
}

float geometry_smith(vec3 n, vec3 v, vec3 l, float roughness)
{
	float n_dot_v = max(dot(n, v), 0.0);
	float n_dot_l = max(dot(n, l), 0.0);
	float ggx2 = geometry_schlick_ggx(n_dot_v, roughness);
	float ggx1 = geometry_schlick_ggx(n_dot_l, roughness);

	return ggx1 * ggx2;
}

vec2 integrate_brdf(float n_dot_v, float roughness)
{
	vec3 v = vec3(sqrt(1.0 - n_dot_v * n_dot_v), 0.0, n_dot_v);

	float a = 0.0;
	float b = 0.0;

	vec3 n = vec3(0.0, 0.0, 1.0);

	const uint SAMPLE_COUNT = 1024u;
	for(uint i = 0u; i < SAMPLE_COUNT; ++i) {
		// Generates a sample vector that's biased towards the
		// preferred alignment direction (importance sampling).
		vec3 h = importance_sample_ggx(i, SAMPLE_COUNT, n, roughness);
		vec3 l = normalize(2.0 * dot(v, h) * h - v);

		float n_dot_l = max(l.z, 0.0);
		float n_dot_h = max(h.z, 0.0);
		float v_dot_h = max(dot(v, h), 0.0);

		if(n_dot_l > 0.0) {
			float g = geometry_smith(n, v, l, roughness);
			float g_vis = (g * v_dot_h) / (n_dot_h * n_dot_v);
			float fc = pow(1.0 - v_dot_h, 5.0);

			a += (1.0 - fc) * g_vis;
			b += fc * g_vis;
		}
	}

	return vec2(a / float(SAMPLE_COUNT), b / float(SAMPLE_COUNT));
}

void main()
{
	o_color = integrate_brdf(v_input.uv.x, v_input.uv.y);
}
