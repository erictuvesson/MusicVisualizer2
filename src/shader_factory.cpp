#include "shader_factory.hpp"

#include <iostream>
#include <regex>

#include "config.hpp"
#include "file_manager.hpp"
#include "shader_parser.hpp"

const char* GetVertexShader() {
	return
		"#version 330\n"
		"in vec2 in_Position;\n"
		"void main(void) {\n"
		"  gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);\n"
		"}";
}


const std::string GetShaderInfoLog(GLuint shader)
{
	int maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	char* log = new char[maxLength];
	glGetShaderInfoLog(shader, maxLength, &maxLength, log);

	return std::string(log);
}

GLuint CompileRawShader(const char* source, const int type)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, (const GLchar**)&source, 0);
	glCompileShader(shader);

	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == false) {
		std::string log = GetShaderInfoLog(shader);
		std::cout << log << '\n';
		return false;
	}

	return shader;
}

std::unique_ptr<ShaderResource> ShaderFactory::CompileShader(ShaderSetup shaderSetup)
{
	ShaderParser parser;
	auto shaderResult = parser.Parse(shaderSetup.FragmentShaderFilepath);
	const char* p_fragment = shaderResult.Result.c_str();

	GLuint vertexShader = CompileRawShader(GetVertexShader(), GL_VERTEX_SHADER);
	if (vertexShader == 0) {
		return nullptr;
	}

	GLuint fragmentShader = CompileRawShader(p_fragment, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) {
		return nullptr;
	}

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindAttribLocation(shaderProgram, 0, "in_Position");

	glLinkProgram(shaderProgram);

	int compiled;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&compiled);

	if (!compiled) {
		std::string log = GetShaderInfoLog(shaderProgram);
		std::cout << log << '\n';
		return nullptr;
	}

	GLint numUniforms = 0;
	glGetProgramInterfaceiv(shaderProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	std::cout << "Uniforms: " << numUniforms << '\n';

	return std::make_unique<ShaderResource>(shaderProgram);
}
