#pragma once

#include "Includes.h"
#include "Settings.h"
#include "ShaderProgram.h"

class SSAO
{
public:
	SSAO();
	~SSAO();

private:
	std::shared_ptr<ShaderProgram> geometry_program;
	std::shared_ptr<ShaderProgram> occlusion_program;
	std::shared_ptr<ShaderProgram> blur_program;
	std::shared_ptr<ShaderProgram> lighting_program;

	GLuint geometry_fbo;
	GLuint ssao_fbo;
	GLuint blur_fbo;

	GLuint create_fbo();
};

