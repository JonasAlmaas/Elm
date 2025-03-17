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

layout (std140, binding = 1) uniform roughness
{
	float u_roughness;
};

struct vertex_output
{
	vec3 frag_pos;
};

layout (location = 0) in vertex_output v_input;
layout (binding = 0) uniform samplerCube u_environment_map;

const float PI = 3.14159265359;

float distribution_ggx(vec3 n, vec3 h, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float n_dot_h = max(dot(n, h), 0.0);
	float n_dot_h2 = n_dot_h*n_dot_h;

	float nom   = a2;
	float denom = (n_dot_h2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

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

vec3 importance_sample_ggx(uint i, uint sample_count, vec3 n, float roughness)
{
	vec2 hammersley = vec2(float(i) / float(sample_count), radical_inverse_vdc(i));

	float a = roughness * roughness;

	float phi = 2.0 * PI * hammersley.x;
	float cos_theta = sqrt((1.0 - hammersley.y) / (1.0 * (a*a - 1.0) * hammersley.y));
	float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	// From spherical coordinates to tangent space cartesian coordinates
	vec3 h = vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);

	vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, n));
	vec3 bitangent = cross(n, tangent);

	vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;
	return normalize(sample_vec);
}

void main()
{
	vec3 n = normalize(v_input.frag_pos);
	vec3 v = n;

	const uint SAMPLE_COUNT = 1024u;
	float total_weight = 0.0;
	vec3 prefilter_color = vec3(0.0);

	for (uint i = 0u; i < SAMPLE_COUNT; ++i) {
		vec3 h = importance_sample_ggx(i, SAMPLE_COUNT, n, u_roughness);
		vec3 l = normalize(2.0 * dot(v, h) * h - v);
		// vec3 l = reflect(-v, h);
		float n_dot_l = max(dot(n, l), 0.0);

		if (n_dot_l > 0.0) {
			float d = distribution_ggx(n, h, u_roughness);
			float n_dot_h = max(dot(n, h), 0.0);
			float v_dot_h = max(dot(v, h), 0.0);
			float pdf = d * n_dot_h / (4.0 * v_dot_h) + 0.0001;

			float resolution = 512.0; // Resolution of the source cubemap (per face)
			float sa_texel = 4.0 * PI / (6.0 * resolution * resolution);
			float sa_sample = 1.0 / (float(SAMPLE_COUNT) * pdf * 0.0001);

			float mip_level = u_roughness == 0.0 ? 0.0 : 0.5 * log2(sa_sample / sa_texel);

			prefilter_color += textureLod(u_environment_map, l, mip_level).rgb * n_dot_l;
			total_weight += n_dot_l;
		}
	}

	prefilter_color = prefilter_color / total_weight;

	o_color = vec4(prefilter_color, 1.0);
}
