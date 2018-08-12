#include "shader_resource.hpp"

#include <iostream>

#include "config.hpp"
#include "file_manager.hpp"

ShaderResource::ShaderResource(GLuint programId)
	: shaderprogram(programId)
{
	iResolutionLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iResolution");
	iTimeLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iTime");
	iTimeDeltaLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iTimeDelta");
	iFrameLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iFrame");
	iChannelTimeLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iChannelTime");
	iMouseLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iMouse");
	iDateLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iDate");
	iSampleRateLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iSampleRate");
	iChannelResolutionLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iChannelResolution");
	iAudioSumLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iAudioSum");
	iAudioTimeLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iAudioTime");
}

ShaderResource::~ShaderResource()
{

}

void ShaderResource::Apply(const ShaderState& state)
{
	glUseProgram(shaderprogram);

	glUniform3fv(iResolutionLocation, 1, &state.iResolution[0]);
	glUniform1f(iTimeLocation, state.iTime);
	glUniform1f(iTimeDeltaLocation, state.iTimeDelta);
	glUniform1f(iFrameLocation, state.iFrame);
	glUniform4fv(iChannelTimeLocation, 4, (const GLfloat*)&state.iChannelTime);
	glUniform4fv(iMouseLocation, 1, (const GLfloat*)&state.iMouse);
	glUniform4fv(iDateLocation, 1, (const GLfloat*)&state.iDate);
	glUniform1f(iSampleRateLocation, state.iSampleRate);
	glUniform3fv(iChannelResolutionLocation, 4, (const GLfloat*)&state.iChannelResolution);
	glUniform1f(iAudioSumLocation, state.iAudioSum);
	glUniform1f(iAudioTimeLocation, state.iAudioTime);
}

