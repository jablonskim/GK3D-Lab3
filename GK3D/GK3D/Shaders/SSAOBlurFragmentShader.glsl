#version 330 core

in vec2 tex_coord;

out float blured_occlusion;

uniform sampler2D occlusion;

uniform int blur_size;

void main() 
{
	vec2 texel_size = 1.0 / vec2(textureSize(occlusion, 0));

	blured_occlusion = 0.0;

	for (int x = 0; x < blur_size; ++x) 
	{
		for (int y = 0; y < blur_size; ++y) 
		{
			vec2 offset = (vec2(-float(blur_size / 2)) + vec2(float(x), float(y))) * texel_size;
			blured_occlusion += texture(occlusion, tex_coord + offset).r;
		}
	}

	blured_occlusion /= float(blur_size * blur_size);
}