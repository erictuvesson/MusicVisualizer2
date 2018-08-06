#include "shader_factory.hpp"

#include <iostream>

#include "config.hpp"
#include "file_manager.hpp"

const char* GetFragmentShaderHeader() {
	return 
		"#version 330\n"
		"precision highp float;\n"
		"uniform vec3 iResolution;\n"
		"uniform float iTime;\n"
		"uniform float iTimeDelta;\n"
		"uniform float iFrame;\n"
		"uniform float iChannelTime[4];\n"
		"uniform vec4 iMouse;\n"
		"uniform vec4 iDate;\n"
		"uniform float iSampleRate;\n"
		"uniform vec3 iChannelResolution[4];\n"
		"uniform float iAudioSum;\n"
		"uniform float iAudioAverage;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 gl_FragColor;\n";
}

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
	std::string fragment = FileManager::ReadFile(shaderSetup.FragmentShaderFilepath).c_str();
	fragment.insert(0, GetFragmentShaderHeader());

	// TODO: Add include functions shader

	const char* p_fragment = fragment.c_str();

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
