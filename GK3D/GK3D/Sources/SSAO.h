#pragma once

#include "Includes.h"
#include "Settings.h"
#include "ShaderProgram.h"
#include "Model.h"

class SSAO
{
public:
	SSAO(int width, int height);
	~SSAO();

	void render(glm::mat4& projection, std::function<void(std::shared_ptr<ShaderProgram>&)> geometry_action);

private:
	int width;
	int height;

	std::shared_ptr<ShaderProgram> geometry_program;
	std::shared_ptr<ShaderProgram> occlusion_program;
	std::shared_ptr<ShaderProgram> blur_program;
	std::shared_ptr<ShaderProgram> lighting_program;

	std::shared_ptr<Model> screen_quad;

	GLuint geometry_fbo;
	GLuint occlusion_fbo;
	GLuint blur_fbo;

	GLuint position_depth_buffer;
	GLuint normal_buffer;
	GLuint color_buffer;
	GLuint noise_buffer;
	GLuint occlusion_buffer;
	GLuint blur_buffer;

	std::vector<glm::vec3> sample_kernel;

	void geometryPass(std::function<void(std::shared_ptr<ShaderProgram>&)> geometry_action);
	void occlusionPass(glm::mat4& projection);
	void blurPass();
	void lightingPass();

	void createGeometryFbo();
	void createOcclusionFbo();
	void createBlurFbo();

	void createPositionDepthBuffer();
	void createNormalBuffer();
	void createColorBuffer();
	void createOcclusionBuffer();
	void createBlurBuffer();

	void createSampleKernel();
	void createNoiseBuffer();

	void initOcclusionBuffers();
	void initLightingBuffers();

	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
};

