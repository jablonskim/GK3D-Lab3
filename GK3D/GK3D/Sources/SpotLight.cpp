#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 col, glm::vec3 pos, glm::vec3 dir, float angle, float soft_angle) :
	Light(col, pos)
{
	setDirection(dir);
	setAngles(angle, soft_angle);
	setFactors(Settings::SpotLightConstantFactor, Settings::SpotLightLinearFactor, Settings::SpotLightQuadraticFactor);
}

SpotLight::~SpotLight()
{
}

void SpotLight::setAngles(float angle, float soft_angle)
{
	inner_angle = glm::cos(glm::radians(angle));
	outer_angle = glm::cos(glm::radians(angle + soft_angle));
}

void SpotLight::setDirection(glm::vec3 dir)
{
	direction = dir;
}

void SpotLight::changeOnOff()
{
	is_on = !is_on;
}

void SpotLight::use(std::shared_ptr<ShaderProgram> &program)
{
	Light::use(program);

	GLint dir_loc = getUniformLocation(program, Settings::ShaderSpotLightsArrayLocationName, Settings::ShaderSpotLightDirectionLocationName);
	glUniform3fv(dir_loc, 1, glm::value_ptr(direction));

	GLint inner_loc = getUniformLocation(program, Settings::ShaderSpotLightsArrayLocationName, Settings::ShaderSpotLightInnerAngleLocationName);
	glUniform1f(inner_loc, inner_angle);

	GLint outer_loc = getUniformLocation(program, Settings::ShaderSpotLightsArrayLocationName, Settings::ShaderSpotLightOuterAngleLocationName);
	glUniform1f(outer_loc, outer_angle);
}

int SpotLight::getId()
{
	static int sid = 0;
	return sid++;
}

const char * SpotLight::getLightArrayLocName()
{
	return Settings::ShaderSpotLightsArrayLocationName;
}
