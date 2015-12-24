#include "SSAO.h"

SSAO::SSAO(int width, int height) :
	width(width),
	height(height)
{
	screen_quad = Model::createPostprocessingQuad();

	geometry_program = ShaderProgram::create(Settings::VertexShaderSSAOGeometryPath, Settings::FragmentShaderSSAOGeometryPath);
	if (!geometry_program)
		exit(EXIT_FAILURE);

	occlusion_program = ShaderProgram::create(Settings::VertexShaderSSAOOcclusionPath, Settings::FragmentShaderSSAOOcclusionPath);
	if (!occlusion_program)
		exit(EXIT_FAILURE);

	blur_program = ShaderProgram::create(Settings::VertexShaderSSAOBlurPath, Settings::FragmentShaderSSAOBlurPath);
	if (!blur_program)
		exit(EXIT_FAILURE);

	lighting_program = ShaderProgram::create(Settings::VertexShaderSSAOLightingPath, Settings::FragmentShaderSSAOLightingPath);
	if (!lighting_program)
		exit(EXIT_FAILURE);

	initOcclusionBuffers();
	initLightingBuffers();
	
	createGeometryFbo();
	createOcclusionFbo();
	createBlurFbo();
	
	createSampleKernel();
	createNoiseBuffer();
}

SSAO::~SSAO()
{
}

void SSAO::render(glm::mat4& projection, std::function<void(std::shared_ptr<ShaderProgram>&)> geometry_action)
{
	geometryPass(geometry_action);
	occlusionPass(projection);
	blurPass();
	lightingPass();
}

void SSAO::geometryPass(std::function<void(std::shared_ptr<ShaderProgram>&)> geometry_action)
{
	glBindFramebuffer(GL_FRAMEBUFFER, geometry_fbo);

	geometry_program->use();

	glUniform1f(geometry_program->getUniformLocation(Settings::ShaderSSAONearPlaneLocationName), Settings::PerspectiveNear);
	glUniform1f(geometry_program->getUniformLocation(Settings::ShaderSSAOFarPlaneLocationName), Settings::PerspectiveFar);

	geometry_action(geometry_program);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::occlusionPass(glm::mat4 & projection)
{
	glBindFramebuffer(GL_FRAMEBUFFER, occlusion_fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	occlusion_program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, position_depth_buffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_buffer);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noise_buffer);

	for (GLuint i = 0; i < Settings::SSAOKernelSize; ++i)
		glUniform3fv(occlusion_program->getUniformLocation(("samples[" + std::to_string(i) + "]").c_str()), 1, &sample_kernel[i][0]);

	glUniformMatrix4fv(occlusion_program->getUniformLocation(Settings::ShaderProjectionMatrixLocationName), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1i(occlusion_program->getUniformLocation("kernel_size"), Settings::SSAOKernelSize);
	glUniform1f(occlusion_program->getUniformLocation("radius"), Settings::SSAORadius);

	glUniform1f(occlusion_program->getUniformLocation("screen_width"), static_cast<float>(width));
	glUniform1f(occlusion_program->getUniformLocation("screen_height"), static_cast<float>(height));

	glUniform1i(occlusion_program->getUniformLocation("noise_size"), Settings::SSAONoiseSize);

	screen_quad->draw(occlusion_program);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::blurPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo);

	glClear(GL_COLOR_BUFFER_BIT);

	blur_program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, occlusion_buffer);

	glUniform1i(blur_program->getUniformLocation("blur_size"), Settings::SSAONoiseSize);

	screen_quad->draw(blur_program);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::lightingPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lighting_program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, position_depth_buffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_buffer);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, color_buffer);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, blur_buffer);

	screen_quad->draw(lighting_program);
}

void SSAO::createGeometryFbo()
{
	glGenFramebuffers(1, &geometry_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, geometry_fbo);

	createPositionDepthBuffer();
	createNormalBuffer();
	createColorBuffer();

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	GLuint rbo_depth;

	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Geometry Framebuffer not complete!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::createOcclusionFbo()
{
	glGenFramebuffers(1, &occlusion_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, occlusion_fbo);

	createOcclusionBuffer();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Occlusion Framebuffer not complete!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::createBlurFbo()
{
	glGenFramebuffers(1, &blur_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, blur_fbo);

	createBlurBuffer();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Blur Framebuffer not complete!" << std::endl;
		exit(EXIT_FAILURE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::createPositionDepthBuffer()
{
	glGenTextures(1, &position_depth_buffer);
	glBindTexture(GL_TEXTURE_2D, position_depth_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_depth_buffer, 0);
}

void SSAO::createNormalBuffer()
{
	glGenTextures(1, &normal_buffer);
	glBindTexture(GL_TEXTURE_2D, normal_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_buffer, 0);
}

void SSAO::createColorBuffer()
{
	glGenTextures(1, &color_buffer);
	glBindTexture(GL_TEXTURE_2D, color_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color_buffer, 0);
}

void SSAO::createOcclusionBuffer()
{
	glGenTextures(1, &occlusion_buffer);
	glBindTexture(GL_TEXTURE_2D, occlusion_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, occlusion_buffer, 0);
}

void SSAO::createBlurBuffer()
{
	glGenTextures(1, &blur_buffer);
	glBindTexture(GL_TEXTURE_2D, blur_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur_buffer, 0);
}

void SSAO::createSampleKernel()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	for (GLuint i = 0; i < Settings::SSAOKernelSize; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		GLfloat scale = static_cast<GLfloat>(i) / static_cast<GLfloat>(Settings::SSAOKernelSize);

		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		sample_kernel.push_back(sample);
	}
}

void SSAO::createNoiseBuffer()
{
	std::vector<glm::vec3> noise_vec;

	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	for (GLuint i = 0; i < Settings::SSAONoiseSize * Settings::SSAONoiseSize; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
		noise_vec.push_back(noise);
	}

	glGenTextures(1, &noise_buffer);
	glBindTexture(GL_TEXTURE_2D, noise_buffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Settings::SSAONoiseSize, Settings::SSAONoiseSize, 0, GL_RGB, GL_FLOAT, &noise_vec[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAO::initOcclusionBuffers()
{
	occlusion_program->use();

	glUniform1i(occlusion_program->getUniformLocation(Settings::ShaderSSAOPositionDepthBufferName), 0);
	glUniform1i(occlusion_program->getUniformLocation(Settings::ShaderSSAONormalBufferName), 1);
	glUniform1i(occlusion_program->getUniformLocation(Settings::ShaderSSAONoiseBufferName), 2);
}

void SSAO::initLightingBuffers()
{
	lighting_program->use();

	glUniform1i(lighting_program->getUniformLocation(Settings::ShaderSSAOPositionDepthBufferName), 0);
	glUniform1i(lighting_program->getUniformLocation(Settings::ShaderSSAONormalBufferName), 1);
	glUniform1i(lighting_program->getUniformLocation(Settings::ShaderSSAOColorBufferName), 2);
	glUniform1i(lighting_program->getUniformLocation(Settings::ShaderSSAOOcclusionBufferName), 3);
}

GLfloat SSAO::lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}
