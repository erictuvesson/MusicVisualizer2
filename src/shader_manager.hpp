#pragma once

struct ShaderInformation
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
};

/**
 * ShaderManager, manages all currently loaded shaders and it's resources.
 */
class ShaderManager
{
public:
	ShaderManager();
	virtual ~ShaderManager();
};
