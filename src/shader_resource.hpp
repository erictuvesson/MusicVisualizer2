#pragma once

#include "config.hpp"

struct ShaderState
{
	float iResolution[3];
	float iTime;
	float iTimeDelta;
	float iFrame;
	float iMouse[4];
	float iSampleRate;
	float iSample[256];

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

	GLuint sampleTexture;

	GLuint iResolutionLocation;
	GLuint iTimeLocation;
	GLuint iTimeDeltaLocation;
	GLuint iFrameLocation;
	GLuint iMouseLocation;
	GLuint iSampleRateLocation;
	GLuint iSampleLocation;
	GLuint iAudioSumLocation;
	GLuint iAudioTimeLocation;
};
