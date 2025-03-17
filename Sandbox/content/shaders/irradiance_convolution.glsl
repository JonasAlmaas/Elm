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
layout (binding = 0) uniform samplerCube u_environment_map;

const float PI = 3.14159265359;

void main()
{
	vec3 n = normalize(v_input.frag_pos);
	vec3 irradiance = vec3(0.0);

	vec3 right = cross(vec3(0.0, 1.0, 0.0), n);
	vec3 up = cross(n, right);

	float sample_delta = 0.025;
	float n_samples = 0.0;

	for (float phi = 0.0; phi < 2.0 * PI; phi += sample_delta) {
		for (float theta = 0.0; theta < 0.5 * PI; theta += sample_delta) {
			// Spherical to cartesian (in tangent space)
			vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// Tangent to world space
			vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * n;

			irradiance += texture(u_environment_map, sample_vec).rgb * cos(theta) * sin(theta);
			++n_samples;
		}
	}

	irradiance *= PI / n_samples;

	o_color = vec4(irradiance, 1.0);
}
