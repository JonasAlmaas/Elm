#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

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

void main()
{
	v_output.uv = a_uv;
	v_output.normal = a_normal;
	v_output.frag_pos = (u_model.transform * vec4(a_position, 1.0)).xyz;

	gl_Position = u_camera.view_projection * u_model.transform * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

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

layout (location = 0) in vertex_output v_input;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
	vec3 position;
} u_camera;

layout (std140, binding = 2) uniform lights
{
	directional_light dir_light;
} u_lights;

layout (binding = 0) uniform sampler2D u_textures[32];

void main()
{
	// Ambient lighting
	vec3 ambient = u_lights.dir_light.ambient_intensity * u_lights.dir_light.ambient_color;

	// Diffuse lighting
	float diff = max(dot(v_input.normal, -u_lights.dir_light.direction), 0.0);
	vec3 diffuse = diff * u_lights.dir_light.color * u_lights.dir_light.intensity;

	// Specular lighting
	float specular_strength = 0.5;
	vec3 view_dir = normalize(u_camera.position - v_input.frag_pos);
	vec3 reflect_dir = reflect(u_lights.dir_light.direction, v_input.normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * u_lights.dir_light.color;

	// Combine results
	vec4 color = texture(u_textures[0], v_input.uv);
	vec3 light = ambient + diffuse + specular;
	color = vec4(color.xyz * light, color.a);

	o_color = color;
}
