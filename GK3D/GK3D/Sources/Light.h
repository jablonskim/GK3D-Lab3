#pragma once

#include "Includes.h"
#include "Settings.h"
#include "ShaderProgram.h"

class Light
{
public:
	Light(glm::vec3 col = glm::vec3(), glm::vec3 pos = glm::vec3());
	virtual ~Light();

	void setPosition(glm::vec3 pos);
	void setColor(glm::vec3 col);
	void setFactors(float constant, float linear, float quadratic);

	virtual void use(std::shared_ptr<ShaderProgram> &program);

protected:
	virtual int getId() = 0;
	GLint getUniformLocation(std::shared_ptr<ShaderProgram> &program, const char* struct_name, const char* field_name);
	virtual const char* getLightArrayLocName() = 0;

	bool is_on;
	glm::vec3 color;

private:
	int light_id;

	glm::vec3 position;

	float constant_factor;
	float linear_factor;
	float quadratic_factor;
};

