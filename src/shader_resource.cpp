#include "shader_resource.hpp"

#include <iostream>

#include "config.hpp"
#include "file_manager.hpp"

ShaderResource::ShaderResource()
{

}

ShaderResource::~ShaderResource()
{

}

bool ShaderResource::Compile(const std::string& vertFile, const std::string& fragFile)
{
	char* infoLog;

	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;
	int maxLength;

	const std::string vertexsource = FileManager::ReadFile(vertFile).c_str();
	const std::string fragmentsource = FileManager::ReadFile(fragFile).c_str();

	const char* p_vertexsource = vertexsource.c_str();
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

void ShaderResource::Apply()
{
	glUseProgram(shaderprogram);
}
