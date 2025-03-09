#type vertex
#version 450 core

layout (location = 0) in vec3 a_position;

layout (std140, binding = 0) uniform camera
{
	mat4 view_projection;
} u_camera;

layout (std140, binding = 1) uniform model
{
	mat4 transform;
} u_model;

void main()
{
	gl_Position = u_camera.view_projection * u_model.transform * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_color;

layout (std140, binding = 2) uniform color
{
	vec4 u_color;
};

void main()
{
	o_color = u_color;
}
