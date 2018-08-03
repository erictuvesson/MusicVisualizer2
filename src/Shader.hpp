#pragma once

typedef unsigned int GLuint;

class Shader
{
private:
	GLuint vertexshader, fragmentshader;
	GLuint shaderprogram;

	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;
	int maxLength;

	char* vertexInfoLog;
	char* fragmentInfoLog;
	char* shaderProgramInfoLog;

public:
	Shader();
	virtual ~Shader();

	bool Compile();
	void Apply();
};

