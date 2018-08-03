#pragma once

#include <string>

typedef unsigned int GLuint;

class Shader
{
private:
	GLuint vertexshader, fragmentshader;
	GLuint shaderprogram;

public:
	Shader();
	virtual ~Shader();

	bool Compile(const std::string& vertFile, const std::string& fragFile);

	void Apply();
};

