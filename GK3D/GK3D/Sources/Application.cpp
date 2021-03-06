#include "Application.h"

Application::Application() :
	is_initialized(false),
	window(nullptr),
	current_width(0),
	current_height(0)
{
	if (!initGLFW())
		return;

	if (!createWindow())
		return;

	Input::instance()->registerKeyboard(window);
	Input::instance()->registerMouse(window);

	Input::instance()->disableCursor(window, true);

	if (!initGLEW())
		return;

	is_initialized = true;
}

Application::~Application()
{
	std::cout << "Cleaning..." << std::endl;
	glfwTerminate();
}

int Application::run()
{
	std::cout << "Starting Application" << std::endl;

	if (!is_initialized)
	{
		std::cerr << "Application is not initialized!" << std::endl;
		return 1;
	}

	glViewport(0, 0, current_width, current_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_CLIP_DISTANCE0);
	glDisable(GL_MULTISAMPLE);

	ssao = std::make_shared<SSAO>(current_width, current_height);

	camera = std::make_shared<Camera>(current_width, current_height);

	createModels();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		Input::instance()->handleInput(camera, [this]() { terrain->swapTextures(); });
		ssao->render(camera->getProjectionMatrix(), [this](auto p) { renderGeometry(p); }, [this](auto p) { renderLight(p); });

		glfwSwapBuffers(window);
	}

	return 0;
}

bool Application::initGLFW()
{
	std::cout << "Initializing GLFW" << std::endl;

	if (glfwInit() == GL_FALSE)
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Settings::OpenGlMinVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Settings::OpenGlMinVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, Settings::WindowResizable ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	return true;
}

bool Application::initGLEW()
{
	std::cout << "Initializing GLEW" << std::endl;

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	glGetError();

	return true;
}

bool Application::createWindow()
{
	std::cout << "Creating window" << std::endl;

	if (Settings::Fullscreen)
	{
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		window = glfwCreateWindow(mode->width, mode->height, Settings::WindowTitle, monitor, nullptr);
	}
	else
	{
		window = glfwCreateWindow(Settings::ScreenWidth, Settings::ScreenHeight, Settings::WindowTitle, nullptr, nullptr);
	}

	glfwGetWindowSize(window, &current_width, &current_height);

	if (window == nullptr)
	{
		std::cerr << "Failed to create window." << std::endl;
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

void Application::createModels()
{
	terrain = Model::createTerrain();
	cube = Model::createCube();

	glm::vec4 tree_color = glm::vec4(0.133333f, 0.545098f, 0.133333f, 1.0f);
	glm::vec4 lamp_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	glm::vec4 bench_color = glm::vec4(0.545098f, 0.270588f, 0.0745098f, 1.0f);

	glm::vec3 tree_translation = glm::vec3(0.f, -0.02f, 0.f);
	glm::vec3 lamp_translation = glm::vec3(0.f, -0.02f, -0.4f);
	glm::vec3 bench_translation = glm::vec3(0.f, -0.02f, 0.f);

	GLfloat tree_scale = 0.5f;
	GLfloat lamp_scale = 0.005f;
	GLfloat bench_scale = 0.002f;

	GLfloat tree_rotate_angle = -90.f;
	GLfloat lamp_rotate_angle = -90.f;
	GLfloat bench_rotate_angle = 0;

	glm::vec3 tree_rotate_pos = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 lamp_rotate_pos = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 bench_rotate_pos = glm::vec3(1.f);

	auto lamp_mesh = Mesh::fromFile(Settings::LampModelPath);
	auto bench_mesh = Mesh::fromFile(Settings::BenchModelPath);
	auto tree_mesh = Mesh::fromFile(Settings::TreeModelPath);


	auto lamp1 = Model::fromMeshes(lamp_mesh);
	lamp1->setColor(lamp_color);
	auto lamp1_trans = glm::translate(glm::mat4(), getVecFromPerlin(0.0f, 0.0f));
	lamp1->setMatrixFromDefaults(lamp1_trans, lamp_translation, lamp_scale, lamp_rotate_angle, lamp_rotate_pos);
	models.push_back(lamp1);

	auto lamp2 = Model::fromMeshes(lamp_mesh);
	lamp2->setColor(lamp_color);
	auto lamp2_trans = glm::translate(glm::mat4(), getVecFromPerlin(3.0f, -1.8f));
	lamp2->setMatrixFromDefaults(lamp2_trans, lamp_translation, lamp_scale, lamp_rotate_angle, lamp_rotate_pos);
	models.push_back(lamp2);

	light = std::make_shared<PointLight>();
	light->setColor(glm::vec3(Settings::PointLightR, Settings::PointLightG, Settings::PointLightB));
	light->setPosition(glm::vec3(0.f, 0.7f + getPerlin(0.f, -0.4f), -0.4f));

	broken_light = std::make_shared<PointLight>();
	broken_light->setBroken();
	broken_light->setPosition(glm::vec3(3.f, 0.7f + getPerlin(3.f, -2.2f), -2.2f));

	
	auto bench1 = Model::fromMeshes(bench_mesh);
	bench1->setColor(bench_color);
	auto bench1_trans = glm::translate(glm::mat4(), getVecFromPerlin(0.f, 0.f));
	bench1->setMatrixFromDefaults(bench1_trans, bench_translation, bench_scale, bench_rotate_angle, bench_rotate_pos);
	models.push_back(bench1);

	auto bench2 = Model::fromMeshes(bench_mesh);
	bench2->setColor(bench_color);
	auto bench2_trans = glm::translate(glm::mat4(), getVecFromPerlin(1.5f, -2.5f));
	auto bench2_rot = glm::rotate(bench2_trans, glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
	bench2->setMatrixFromDefaults(bench2_rot, bench_translation, bench_scale, bench_rotate_angle, bench_rotate_pos);
	models.push_back(bench2);

	auto bench3 = Model::fromMeshes(bench_mesh);
	bench3->setColor(bench_color);
	auto bench3_trans = glm::translate(glm::mat4(), getVecFromPerlin(2.8f, 0.2f));
	auto bench3_rot = glm::rotate(bench3_trans, glm::radians(135.f), glm::vec3(0.f, 1.f, 0.f));
	bench3->setMatrixFromDefaults(bench3_rot, bench_translation, bench_scale, bench_rotate_angle, bench_rotate_pos);
	models.push_back(bench3);


	srand(8);

	for (int i = 0; i < 30; ++i)
	{
		auto tree = Model::fromMeshes(tree_mesh);
		tree->setColor(tree_color);
		GLfloat z_pos = (static_cast<GLfloat>(rand()) / RAND_MAX * 14) - 7;
		GLfloat x_pos = (static_cast<GLfloat>(rand()) / RAND_MAX * 14) - 7;
		glm::mat4 translation = glm::translate(glm::mat4(), getVecFromPerlin(x_pos, z_pos));
		tree->setMatrixFromDefaults(translation, tree_translation, tree_scale, tree_rotate_angle, tree_rotate_pos);
		models.push_back(tree);
	}
	
}

void Application::renderFrame(bool allow_wireframe)
{
	/*glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->use();
	camera->use(allow_wireframe);

	terrain->draw();
	cube->draw();
	std::for_each(std::cbegin(models), std::cend(models), [](auto model) { model->draw(); });

	light->use();
	broken_light->use();*/
}

void Application::renderGeometry(std::shared_ptr<ShaderProgram> &program)
{
	camera->useGeometry(program);

	terrain->draw(program);
	cube->draw(program);

	std::for_each(std::cbegin(models), std::cend(models), [&program](auto model) { model->draw(program); });
}

void Application::renderLight(std::shared_ptr<ShaderProgram>& program)
{
	camera->useLight(program);

	light->use(program);
	broken_light->use(program);
}

float Application::getPerlin(float x, float y)
{
	x = x / Settings::TerrainScaleFactor + 1.0f;
	y = -y / Settings::TerrainScaleFactor + 1.0f;
	return 0.05f * static_cast<float>(perlin.GetValue(x, y, 0.5)) * Settings::TerrainScaleFactor;
}

glm::vec3 Application::getVecFromPerlin(float x, float z)
{
	return glm::vec3(x, getPerlin(x, z), z);
}
