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
	iMouseLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iMouse");
	iSampleRateLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iSampleRate");
	iAudioSumLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iAudioSum");
	iAudioTimeLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iAudioTime");
	iSampleLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iSample");
	iThemeColorLocation = glGetProgramResourceLocation(shaderprogram, GL_UNIFORM, "iThemeColor");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &sampleTexture);

	glBindTexture(GL_TEXTURE_2D, sampleTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
}

ShaderResource::~ShaderResource()
{

}

void ShaderResource::Apply(const ShaderState& state)
{
	glUseProgram(shaderprogram);

	GLfloat data[256];
	for (size_t i = 0; i < 256; i++) {
		data[i] = 1.0f;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sampleTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 256, 1, 0, GL_RED, GL_FLOAT, data);

	glUniform1i(iSampleLocation, 0);
	glUniform3fv(iResolutionLocation, 1, &state.iResolution[0]);
	glUniform1f(iTimeLocation, state.iTime);
	glUniform1f(iTimeDeltaLocation, state.iTimeDelta);
	glUniform1f(iFrameLocation, state.iFrame);
	glUniform4fv(iMouseLocation, 1, (const GLfloat*)&state.iMouse);
	glUniform1f(iSampleRateLocation, state.iSampleRate);
	glUniform1f(iAudioSumLocation, state.iAudioSum);
	glUniform1f(iAudioTimeLocation, state.iAudioTime);

	glUniformMatrix4fv(iThemeColorLocation, 1, GL_FALSE, &state.iThemeColor[0][0]);
}

