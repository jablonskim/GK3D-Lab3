#include "SSAO.h"

SSAO::SSAO()
{
	geometry_program = ShaderProgram::create(Settings::VertexShaderSSAOGeometryPath, Settings::FragmentShaderSSAOGeometryPath);

	if (!geometry_program)
	{
		exit(EXIT_FAILURE);
	}

	/*occlusion_program = ShaderProgram::create(Settings::VertexShaderSSAOOcclusionPath, Settings::FragmentShaderSSAOOcclusionPath);
	blur_program = ShaderProgram::create(Settings::VertexShaderSSAOBlurPath, Settings::FragmentShaderSSAOBlurPath);
	lighting_program = ShaderProgram::create(Settings::VertexShaderSSAOLightingPath, Settings::FragmentShaderSSAOLightingPath);*/
}

SSAO::~SSAO()
{
}

void SSAO::render(std::function<void(std::shared_ptr<ShaderProgram>&)> geometry_action)
{
	geometry_program->use();

	geometry_action(geometry_program);
}

GLuint SSAO::create_fbo()
{
	GLuint fbo;



	return fbo;
}
