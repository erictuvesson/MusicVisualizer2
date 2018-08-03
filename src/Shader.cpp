#include "shader.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <fstream>
#include <streambuf>
#include <iostream>

Shader::Shader()
{

}

Shader::~Shader()
{

}

bool Shader::Compile(const std::string& vertFile, const std::string& fragFile)
{
	char* infoLog;

	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;
	int maxLength;

	std::ifstream vert(vertFile);
	std::string vertexsource((std::istreambuf_iterator<char>(vert)), std::istreambuf_iterator<char>());
	const char* p_vertexsource = vertexsource.c_str();

	std::ifstream frag(fragFile);
	std::string fragmentsource((std::istreambuf_iterator<char>(frag)), std::istreambuf_iterator<char>());
	const char* p_fragmentsource = fragmentsource.c_str();

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&p_vertexsource, 0);
	glCompileShader(vertexshader);

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == false)
	{
		glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = (char*)malloc(maxLength);
		glGetShaderInfoLog(vertexshader, maxLength, &maxLength, infoLog);
		std::cout << infoLog;
		delete infoLog;
		return false;
	}

	glShaderSource(fragmentshader, 1, (const GLchar**)&p_fragmentsource, 0);
	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == false)
	{
		glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = (char*)malloc(maxLength);
		glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, infoLog);
		std::cout << infoLog;
		delete infoLog;
		return false;
	}

	shaderprogram = glCreateProgram();

	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);

	glBindAttribLocation(shaderprogram, 0, "in_Position");

	glLinkProgram(shaderprogram);

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
	if (!IsLinked)
	{
		glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);
		infoLog = (char *)malloc(maxLength);
		glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, infoLog);
		std::cout << infoLog;
		delete infoLog;
		return false;
	}

	return true;
}

void Shader::Apply()
{
	glUseProgram(shaderprogram);
}