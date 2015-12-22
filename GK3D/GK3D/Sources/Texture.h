#pragma once

#include "Includes.h"
#include "Settings.h"
#include "ShaderProgram.h"

class Texture
{
public:
	static std::shared_ptr<Texture> fromFile(const char* filename, bool no_repeat = false);
	void use(std::shared_ptr<ShaderProgram> &program, int num);
	~Texture();

private:
	Texture();
	bool loadFromFile(const char* filename, bool no_repeat);

	GLuint texture;
	int width;
	int height;
};

