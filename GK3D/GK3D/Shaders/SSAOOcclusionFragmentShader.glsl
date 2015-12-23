#version 330 core

in vec2 tex_coord;

out float occlusion;

uniform sampler2D position_depth_buffer;
uniform sampler2D normal_buffer;
uniform sampler2D noise_buffer;

uniform mat4 projection_matrix;

uniform vec3 samples[256];
uniform int kernel_size;
uniform float radius;

// TODO: from config
const vec2 noise_scale = vec2(1920.0f / 4.0f, 1080.0f / 4.0f);

void main()
{
	vec3 fragment_position = texture(position_depth_buffer, tex_coord).xyz;
	vec3 normal = texture(normal_buffer, tex_coord).rgb;
	vec3 random_vector = texture(noise_buffer, tex_coord * noise_scale).xyz;

	vec3 tg = normalize(random_vector - normal * dot(random_vector, normal));
	vec3 btg = cross(normal, tg);
	mat3 tbn = mat3(tg, btg, normal);

	occlusion = 0.0f;

	for(int i = 0; i < kernel_size; ++i)
	{
		vec3 smpl = tbn * samples[i];
		smpl = fragment_position + smpl * radius;

		vec4 offset = projection_matrix * vec4(smpl, 1.0f);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sample_depth = -texture(position_depth_buffer, offset.xy).w;
		float range_check = smoothstep(0.0, 1.0, radius / abs(fragment_position.z - sample_depth));

		occlusion += (sample_depth >= smpl.z ? 1.0 : 0.0) * range_check;
	}

	occlusion = 1 - (occlusion / kernel_size);
}