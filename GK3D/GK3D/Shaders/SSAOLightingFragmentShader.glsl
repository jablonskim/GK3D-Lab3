#version 330 core

in vec2 tex_coord;

out vec4 color;

uniform sampler2D position_depth_buffer;
uniform sampler2D normal_buffer;
uniform sampler2D color_buffer;
uniform sampler2D occlusion_buffer;

void main()
{
	vec3 fragment_position = texture(position_depth_buffer, tex_coord).rgb;
	vec3 normal = texture(normal_buffer, tex_coord).rgb;
	vec4 fragment_color = texture(color_buffer, tex_coord);
	float fragment_occlusion = texture(occlusion_buffer, tex_coord).r;

	// TODO

	color = fragment_color;
}