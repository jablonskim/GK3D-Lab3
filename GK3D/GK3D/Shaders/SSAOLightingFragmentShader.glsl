#version 330 core

#define POINT_LIGHTS 2
#define SPOT_LIGHTS 1

struct PointLight
{
	vec3 position;
	vec3 color;
	
	float constant_factor;
	float linear_factor;
	float quadratic_factor;
};

struct SpotLight
{
	vec3 position;
	vec3 color;
	vec3 direction;

	float inner_cut_angle;
	float outer_cut_angle;

	float constant_factor;
	float linear_factor;
	float quadratic_factor;
};

in vec2 tex_coord;

out vec4 color;

uniform sampler2D position_depth_buffer;
uniform sampler2D normal_buffer;
uniform sampler2D color_buffer;
uniform sampler2D occlusion_buffer;

uniform mat4 view_matrix;
uniform bool ssao_enabled;

uniform vec3 ambient_color;
uniform float ambient_strength;
uniform float specular_strength;
uniform int specular_shininess;

uniform PointLight point_lights[POINT_LIGHTS];
uniform SpotLight spot_lights[SPOT_LIGHTS];

float calculate_diffuse(vec3 model_normal, vec3 light_direction)
{
	return max(dot(model_normal, light_direction), 0.0f);
}

float calculate_specular(vec3 model_normal, vec3 light_direction, vec3 view_direction)
{
	vec3 reflect_direction = reflect(-light_direction, model_normal);
	float ref_angle = max(dot(view_direction, reflect_direction), 0.0f);

	return specular_strength * pow(ref_angle, specular_shininess);
}

float calculate_attenuation(vec3 l_pos, float constant, float linear, float quadratic, vec3 fragment_position)
{
	float dist = length(l_pos - fragment_position);
	float fac = constant + (dist * linear) + (dist * dist * quadratic);

	return 1.0f / fac;
}

vec3 to_view_space(vec3 vector)
{
	vec4 new_vec = view_matrix * vec4(vector, 1.0f);
	//new_vec.xyz /= new_vec.w;

	return new_vec.xyz;
}

vec3 calculate_spotlight(SpotLight light, vec3 model_normal, vec3 view_direction, vec3 fragment_position)
{
	vec3 light_direction = normalize(to_view_space(light.position) - fragment_position);

	float theta = dot(normalize(to_view_space(light.direction)), normalize(-light_direction));
	float eps = light.inner_cut_angle - light.outer_cut_angle;
	float intensity = clamp((theta - light.outer_cut_angle) / eps, 0.0f, 1.0f);

	float diffuse = calculate_diffuse(model_normal, light_direction);
	float specular = calculate_specular(model_normal, light_direction, view_direction);
	
	//float attenuation = calculate_attenuation(light.position, light.constant_factor, light.linear_factor, light.quadratic_factor);

	return light.color * ((specular + diffuse) * intensity /** attenuation*/);
}

vec3 calculate_pointlight(PointLight light, vec3 model_normal, vec3 view_direction, vec3 fragment_position)
{
	vec3 light_direction = normalize(to_view_space(light.position) - fragment_position);

	float diffuse = calculate_diffuse(model_normal, light_direction);
	float specular = calculate_specular(model_normal, light_direction, view_direction);

	float attenuation = calculate_attenuation(to_view_space(light.position), light.constant_factor, light.linear_factor, light.quadratic_factor, fragment_position);

	return light.color * (specular + diffuse) * attenuation;
}

void main()
{
	vec3 fragment_position = texture(position_depth_buffer, tex_coord).rgb;
	vec3 normal = texture(normal_buffer, tex_coord).rgb;
	vec4 fragment_color = texture(color_buffer, tex_coord);
	float fragment_occlusion = ssao_enabled ? texture(occlusion_buffer, tex_coord).r : 1.0f;

	vec3 view_direction = normalize(-fragment_position);

	vec3 light_result = ambient_color * ambient_strength * fragment_occlusion;

	/*for(int i = 0; i < SPOT_LIGHTS; ++i)
		light_result += calculate_spotlight(spot_lights[i], normal, view_direction, fragment_position);*/

	for(int i = 0; i < POINT_LIGHTS; ++i)
		light_result += calculate_pointlight(point_lights[i], normal, view_direction, fragment_position) * fragment_occlusion;

	color = fragment_color * vec4(light_result, 1.0f);
}