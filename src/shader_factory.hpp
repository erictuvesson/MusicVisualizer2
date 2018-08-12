#pragma once

#include <memory>

#include "shader_resource.hpp"

struct ShaderSetup
{
	std::string FragmentShaderFilepath;
};

/**
 * ShaderFactory, load and complies all shaders used.
 */
class ShaderFactory
{
public:
	static std::unique_ptr<ShaderResource> CompileShader(ShaderSetup shaderSetup);
};
