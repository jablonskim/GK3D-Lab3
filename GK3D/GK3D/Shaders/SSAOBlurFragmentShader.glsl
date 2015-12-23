#version 330 core

in vec2 tex_coord;

// TODO
//out float blured_occlusion;
out vec4 color;

uniform sampler2D occlusion;

// TODO: config
const int blur_size = 4;

void main() 
{
	vec2 texel_size = 1.0 / vec2(textureSize(occlusion, 0));

	float result = 0.0;

	for (int x = 0; x < blur_size; ++x) 
	{
		for (int y = 0; y < blur_size; ++y) 
		{
			// TODO: config
			vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texel_size;
			result += texture(occlusion, tex_coord + offset).r;
		}
	}

	result /= float(blur_size * blur_size);
	//blured_occlusion = result;
	color = vec4(result, result, result, 1.0f);
}