#include "Model.h"

Model::Model(std::vector<std::shared_ptr<Mesh>> meshes) :
	model_meshes(meshes),
	swap_texture(nullptr),
	disable_properties(false)
{
	color = glm::vec4(0.f, 0.f, 0.f, 0.f);
	setMatrix(glm::mat4());

	std::cout << "Creating model..." << std::endl;
}

void Model::useColor(std::shared_ptr<ShaderProgram> &program)
{
	glUniform4fv(program->getUniformLocation(Settings::ShaderModelColorLocationName), 1, glm::value_ptr(color));
}

void Model::useMatrix(std::shared_ptr<ShaderProgram> &program)
{
	GLint model_mat = program->getUniformLocation(Settings::ShaderModelMatrixLocationName);
	glUniformMatrix4fv(model_mat, 1, GL_FALSE, glm::value_ptr(model_matrix));

	GLint normal_mat = program->getUniformLocation(Settings::ShaderNormalMatrixLocationName);
	glUniformMatrix3fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));
}

void Model::useTextures(std::shared_ptr<ShaderProgram> &program)
{
	int size = textures.size();

	GLint tex_num = program->getUniformLocation(Settings::ShaderNumTexturesLocationName);
	glUniform1i(tex_num, size);

	for (int i = 0; i < size; ++i)
	{
		textures[i]->use(program, i);
	}
}

std::shared_ptr<Model> Model::fromMeshes(std::vector<std::shared_ptr<Mesh>> meshes)
{
	return std::shared_ptr<Model>(new Model(meshes));
}

std::shared_ptr<Model> Model::createTerrain()
{
	auto m = Model::fromMeshes(Mesh::createTerrain());

	m->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	auto rotated = glm::rotate(m->model_matrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
	auto scaled = glm::scale(rotated, glm::vec3(Settings::TerrainScaleFactor));
	m->setMatrix(scaled);

	auto texture = Texture::fromFile(Settings::BaseGrassTexture);
	m->addTexture(texture);

	auto leave = Texture::fromFile(Settings::LeavesTexture, true);
	m->addTexture(leave);

	auto extra_texture = Texture::fromFile(Settings::ExtraGrassTexture);
	m->setSwapTexture(extra_texture);

	return m;
}

std::shared_ptr<Model> Model::createCube()
{
	auto m = Model::fromMeshes(Mesh::createCube());

	m->setColor(glm::vec4(1.f, 1.f, 0.f, 1.f));
	auto rotated = glm::rotate(m->model_matrix, glm::radians(10.f), glm::vec3(-1.f, 0.f, 0.5f));
	auto scaled = glm::scale(rotated, glm::vec3(0.2f));
	auto translated = glm::translate(scaled, glm::vec3(-8.f, 1.2f, 3.f));
	m->setMatrix(translated);

	auto texture = Texture::fromFile(Settings::CubeTexturePath);
	m->addTexture(texture);

	return m;
}

std::shared_ptr<Model> Model::createSkybox()
{
	auto m = Model::fromMeshes(Mesh::createCube());
	m->disable_properties = true;

	return m;
}

std::shared_ptr<Model> Model::createPostprocessingQuad()
{
	auto m = Model::fromMeshes(Mesh::createPostprocessingQuad());
	m->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	m->disable_properties = true;

	return m;
}

std::shared_ptr<Model> Model::createSphere()
{
	auto m = Model::fromMeshes(Mesh::createSphere());

	auto scaled = glm::scale(glm::mat4(), glm::vec3(0.6f));
	auto translated = glm::translate(scaled, glm::vec3(-6.f, 2.f, -6.f));
	m->setMatrix(translated);

	return m;
}

Model::~Model()
{
	std::cout << "Destroying model..." << std::endl;
}

void Model::draw(std::shared_ptr<ShaderProgram> &program)
{
	if (!disable_properties)
	{
		useColor(program);
		useMatrix(program);
		useTextures(program);
	}

	for(auto & m : model_meshes)
		m->draw();
}

void Model::setColor(glm::vec4 col)
{
	color = col;
}

void Model::setMatrix(glm::mat4 matrix)
{
	model_matrix = matrix;
	normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrix)));
}

void Model::setMatrixFromDefaults(glm::mat4 base, glm::vec3 translation, GLfloat scale, GLfloat angle, glm::vec3 axis)
{
	auto translated = glm::translate(base, translation);
	auto scaled = glm::scale(translated, glm::vec3(scale));
	auto rotated = glm::rotate(scaled, glm::radians(angle), axis);

	setMatrix(rotated);
}

void Model::addTexture(std::shared_ptr<Texture> texture)
{
	if (textures.size() >= Settings::TexturesCount)
		return;

	textures.push_back(texture);
}

void Model::setSwapTexture(std::shared_ptr<Texture> texture)
{
	swap_texture = texture;
}

void Model::swapTextures()
{
	if (swap_texture != nullptr && textures.size() > 0)
	{
		auto tmp = swap_texture;
		swap_texture = textures[0];
		textures[0] = tmp;
	}
}
