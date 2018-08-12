#pragma once

#include "config.hpp"

struct ShaderState
{
	float iResolution[3];
	float iTime;
	float iTimeDelta;
	float iFrame;
	float iChannelTime[4];
	float iMouse[4];
	float iDate[4];
	float iSampleRate;
	float iChannelResolution[3][4];
	
	float iAudioSum;
	float iAudioTime;
};

/**
 * ShaderResource, stores a shader and it's state.
 */
class ShaderResource
{
public:
	ShaderResource(GLuint programId);
	virtual ~ShaderResource();

	void Apply(const ShaderState& state);

private:
	GLuint shaderprogram;

	GLuint iResolutionLocation;
	GLuint iTimeLocation;
	GLuint iTimeDeltaLocation;
	GLuint iFrameLocation;
	GLuint iChannelTimeLocation;
	GLuint iMouseLocation;
	GLuint iDateLocation;
	GLuint iSampleRateLocation;
	GLuint iChannelResolutionLocation;
	GLuint iAudioSumLocation;
	GLuint iAudioTimeLocation;
};
