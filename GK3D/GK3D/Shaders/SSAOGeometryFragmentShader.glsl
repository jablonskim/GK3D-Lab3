#version 330 core

#define NUM_TEXTURES 2

layout (location = 0) out vec4 position_depth_buffer;
layout (location = 1) out vec3 normal_buffer;
layout (location = 2) out vec4 color_buffer;

in vec3 normal;
in vec3 fragment_position;
in vec2 tex_coord;

uniform float near_plane;
uniform float far_plane;

uniform vec4 model_color;

uniform sampler2D textures[NUM_TEXTURES];
uniform int num_textures;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{    
    position_depth_buffer.xyz = fragment_position;
    position_depth_buffer.a = LinearizeDepth(gl_FragCoord.z);
    
    normal_buffer = normalize(normal);

    color_buffer = model_color;

	for(int i = 0; i < num_textures; ++i)
	{
		vec4 src_color = texture(textures[i], tex_coord);
		float src_alpha = src_color.w;

		color_buffer = src_alpha * src_color + (1.0 - src_alpha) * color_buffer;
	}
}