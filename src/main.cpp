#include "app.hpp"

#include <iostream>

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

int main(int argc, char *argv[])
{
	App app;
	
	const int result = app.Run();
	std::cin.get();

	return result;
}
