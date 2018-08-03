#pragma once

#include "config.hpp"

/**
 * ShaderResource, stores a shader and it's state.
 */
class ShaderResource
{
public:
	ShaderResource();
	virtual ~ShaderResource();

	bool Compile(const std::string& vertFile, const std::string& fragFile);

	void Apply();

private:
	GLuint vertexshader, fragmentshader;
	GLuint shaderprogram;
};

