#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in int a_texture_slot; // TODO: Remove

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
	mat3 normal_matrix = transpose(inverse(mat3(u_model.transform)));

	v_output.uv = a_uv;
	v_output.normal = normalize(normal_matrix * a_normal);
	v_output.frag_pos = (u_model.transform * vec4(a_position, 1.0)).xyz;

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
	vec3 color;
};

struct point_light
{
	vec3 position;
	vec3 color;
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
	int point_light_count;
	point_light point_lights[MAX_POINT_LIGHTS];
} u_lights;

layout (binding = 0) uniform sampler2D u_albedo_map;
// u_normal_map;
// u_roughness_map;
// u_ao_map;
// u_metalness_map;
// u_alpha_map; // Maybe
layout (binding = 6) uniform samplerCube u_irradiance_map;
layout (binding = 7) uniform samplerCube u_prefilter_map;
layout (binding = 8) uniform sampler2D u_brdf_lut;

const float PI = 3.14159265359;

float distribute_ggx(float n_dot_h, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float denom = n_dot_h * n_dot_h * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return a2 / max(denom, 0.0000001); // Prevent divide by zero
}

float geometry_smith(float n_dot_v, float n_dot_l, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	float ggx1 = n_dot_v / (n_dot_v * (1.0 - k) + k); // Schlick GGX
	float ggx2 = n_dot_l / (n_dot_l * (1.0 - k) + k);
	return ggx1 * ggx2;
}

vec3 fresnel_schlick(float h_dot_v, vec3 f0)
{
	// 'f0' in range 0 to 1
	// return range of 'f0' to 1
	// Increases as 'h_dot_v' decreases (more reflective when the surface is view at larger angles)
	return f0 + (1.0 - f0) * pow(1.0 - h_dot_v, 5.0);
}

vec3 fresnel_schlick_roughness(float h_dot_v, vec3 f0, float roughness)
{
	// Greater roughness less fresnel
	return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(1.0 - h_dot_v, 5.0);
}

vec3 calculate_lighting(
	vec3 n,
	vec3 v,
	vec3 albedo,
	float metalness,
	float roughness,
	vec3 base_reflectivity,
	vec3 light_dir,
	vec3 radiance)
{
	vec3 l = normalize(light_dir); // Light vector
	vec3 h = normalize(v + l); // Half way bisect vector

	// Cook-Torrance BSDF (Bidirectional Reflectance Distribution Function)
	float n_dot_v = max(dot(n, v), 0.0000001); // Prevent divide by zero
	float n_dot_l = max(dot(n, l), 0.0000001);
	float h_dot_v = max(dot(h, v), 0.0);
	float n_dot_h = max(dot(n, h), 0.0);

	float d = distribute_ggx(n_dot_h, roughness); // Larger the more micro-facets alight to h (normal distribution function)
	float g = geometry_smith(n_dot_v, n_dot_l, roughness); // Smaller the more micro-facets shadowed by other micro-facets
	vec3 f = fresnel_schlick(h_dot_v, base_reflectivity);

	vec3 specular = (d * g * f) / (4.0 * n_dot_v * n_dot_l);

	// For energy conservation, the diffuse and specular light can't
	// be above 1.0 (unless the surface emits light); to preserve this
	// relationship the diffuse component (kd) should equal 1.0 - ks.
	vec3 kd = vec3(1.0) - f; // f equals ks

	// Multiplying kd by the inverse metalness such the only non-metals
	// have diffuse lighting, or a linear blend if partly metal
	// (pure metals have no diffuse light).
	kd *= 1.0 - metalness;

	// Note that 1) Angle or light to surface affects specular, not just diffuse
	//           2) We mix albedo with diffuse, but not specular
	return (kd * albedo / PI + specular) * radiance * n_dot_l;
}

void main()
{
	// TODO: Get from texture maps
	vec3 albedo = texture(u_albedo_map, v_input.uv).rgb;
	float alpha = 1.0;
	float metalness = 0.0;
	float roughness = 0.3;
	float ao = 1.0;

	// TODO: I don't know if this is the 'PBR' way
	roughness = max(roughness, 0.04); // Make sure we don't loose the specular reflections

	// TODO: Get normal from normal map

	vec3 n = normalize(v_input.normal); // Normal vector
	vec3 v = normalize(u_camera.position - v_input.frag_pos); // View vector

	// Calculate reflectance at normal incidence; if a dia-electric (like plastic) use base_reflectivity
	// of 0.04 and if it's a metal, use the albedo color as base_reflectivity (metallic workflow)
	vec3 base_reflectivity = mix(vec3(0.04), albedo, metalness);

	// Reflectance equation
	vec3 lo = vec3(0.0);

	// Directional light
	lo += calculate_lighting(
		n,
		v,
		albedo,
		metalness,
		roughness,
		base_reflectivity,
		-u_lights.dir_light.direction,
		u_lights.dir_light.color);

	// Point lights
	for (int i = 0; i < u_lights.point_light_count; ++i) {
		float distance = length(u_lights.point_lights[i].position - v_input.frag_pos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = u_lights.point_lights[i].color * attenuation;

		lo += calculate_lighting(
			n,
			v,
			albedo,
			metalness,
			roughness,
			base_reflectivity,
			u_lights.point_lights[i].position - v_input.frag_pos,
			radiance);
	}

	float n_dot_v = max(dot(n, v), 0.0000001); // Prevent divide by zero

	// Get ambient term from IBL
	vec3 f = fresnel_schlick_roughness(n_dot_v, base_reflectivity, roughness);
	vec3 kd = (1.0 - f) * (1.0 - metalness);
	vec3 diffuse = texture(u_irradiance_map, n).rgb * albedo * kd;

	// Sample both the pre-filter map and the BRDF lut and combine them together
	// as per the split-sum approximation to get the IBL specular
	const float MAX_REFLECTION_LOD = 4.0;
	vec3 prefiltered_color = textureLod(u_prefilter_map, reflect(-v, n), roughness * MAX_REFLECTION_LOD).rgb;
	vec2 brdf = texture(u_brdf_lut, vec2(n_dot_v, roughness)).xy;
	vec3 specular = prefiltered_color * (f * brdf.x + brdf.y);

	vec3 ambient = (diffuse + specular) * ao;

	vec3 color = ambient + lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// Gamma correction
	color = pow(color, vec3(1.0 / 2.2));

	o_color = vec4(color, alpha);

	// -- Debug --
	// o_color = vec4(texture(u_irradiance_map, n).rgb, alpha); // Irradiance map
	// o_color = vec4(textureLod(u_prefilter_map, n, 1).rgb, alpha); // Prefilter map
	//o_color = vec4((n + vec3(1.0, 1.0, 1.0)) * 0.5, 1.0); // Surface normal debugging
}
