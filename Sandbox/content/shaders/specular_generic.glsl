#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in int a_texture_slot;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
	vec3 position;
} u_camera;

layout (std140, binding = 1) uniform model
{
	mat4 transform;
} u_model;

struct vertex_output
{
	vec2 uv;
	vec3 normal;
	vec3 frag_pos;
};

layout (location = 0) out vertex_output v_output;
layout (location = 3) out flat int v_texture_slot;

void main()
{
	v_output.uv = a_uv;
	v_output.normal = a_normal;
	v_output.frag_pos = (u_model.transform * vec4(a_position, 1.0)).xyz;
	v_texture_slot = a_texture_slot;

	gl_Position = u_camera.view_projection * u_model.transform * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

const int MAX_POINT_LIGHTS = 4;

layout (location = 0) out vec4 o_color;

struct vertex_output
{
	vec2 uv;
	vec3 normal;
	vec3 frag_pos;
};

struct directional_light
{
	vec3 direction;
	float intensity;
	vec3 color;
	float ambient_intensity;
	vec3 ambient_color;
};

struct point_light
{
	vec3 position;
	float intensity;
	vec3 color;
	// Attenuation
	float constant;
	float linear;
	float quadratic;
};

layout (location = 0) in vertex_output v_input;
layout (location = 3) in flat int v_texture_slot;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
	vec3 position;
} u_camera;

layout (std140, binding = 2) uniform lights
{
	directional_light dir_light;
	int point_light_count;
	point_light point_lights[MAX_POINT_LIGHTS];
} u_lights;

layout (binding = 0) uniform sampler2D u_textures[32];

vec3 calc_light(
	vec3 normal,
	vec3 light_dir,
	vec3 light_color,
	float light_intensity,
	vec3 ambient_color,
	float ambient_intensity)
{
	vec3 color = light_color * light_intensity;

	vec3 ambient = ambient_color * ambient_intensity;

	// Diffuse lighting
	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = diff * color;

	// Specular lighting
	float specular_strength = 0.5;
	vec3 view_dir = normalize(u_camera.position - v_input.frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * color;

	return ambient + diffuse + specular;
}

vec3 calc_dir_light(vec3 normal)
{
	return calc_light(
		normal,
		-u_lights.dir_light.direction,
		u_lights.dir_light.color,
		u_lights.dir_light.intensity,
		u_lights.dir_light.ambient_color,
		u_lights.dir_light.ambient_intensity);
}

vec3 calc_point_light(vec3 normal, point_light light)
{
	vec3 light_dir = normalize(light.position - v_input.frag_pos);

	vec3 res = calc_light(
		normal,
		light_dir,
		light.color,
		light.intensity,
		light.color,
		0.1);

	float distance = length(light.position - v_input.frag_pos);
	float attenuation = light.constant + light.linear * distance + light.quadratic * (distance * distance);

	return res / attenuation;
}

void main()
{
	vec3 normal = normalize(v_input.normal);

	vec3 light = calc_dir_light(normal);

	for (int i = 0; i < u_lights.point_light_count; ++i) {
		light += calc_point_light(normal, u_lights.point_lights[i]);
	}

	vec4 color = texture(u_textures[v_texture_slot], v_input.uv);
	o_color = vec4(color.rgb * light, color.a);
}
