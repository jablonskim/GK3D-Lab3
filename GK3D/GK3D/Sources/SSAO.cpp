#include "SSAO.h"

SSAO::SSAO()
{
	geometry_program = ShaderProgram::create(Settings::VertexShaderSSAOGeometryPath, Settings::FragmentShaderSSAOGeometryPath);
	occlusion_program = ShaderProgram::create(Settings::VertexShaderSSAOOcclusionPath, Settings::FragmentShaderSSAOOcclusionPath);
	blur_program = ShaderProgram::create(Settings::VertexShaderSSAOBlurPath, Settings::FragmentShaderSSAOBlurPath);
	lighting_program = ShaderProgram::create(Settings::VertexShaderSSAOLightingPath, Settings::FragmentShaderSSAOLightingPath);

	if (!geometry_program || !occlusion_program || !blur_program || !lighting_program)
	{
		exit(EXIT_FAILURE);
	}
}

SSAO::~SSAO()
{
}

GLuint SSAO::create_fbo()
{
	GLuint fbo;



	return fbo;
}
