#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

#include "Recorder.hpp"
#include "FullscreenQuad.hpp"
#include "Shader.hpp"

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	800

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
	SDL_Window* window;
	SDL_GLContext context;
	ShaderInformation shaderInfo;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != NULL) {
		return EXIT_FAILURE;
	}

	// Set the desired OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window using SDL
	window = SDL_CreateWindow("Skoog",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	Recorder recorder;

	auto devices = recorder.GetDevices();
	recorder.Listen(devices[devices.size() - 1]);

	if (window == NULL) {
		return EXIT_FAILURE;
	}

	// Create the OpenGL context for the window using SDL
	context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		return EXIT_FAILURE;
	}

	// Enable glew experimental, this enables some more OpenGL extensions.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		return EXIT_FAILURE;
	}

	// Application Variables
	bool isRunning = true;

	FullscreenQuad quad;
	
	Shader shader;
	if (!shader.Compile()) {
		return EXIT_FAILURE;
	}
	shader.Apply();

	// Set some OpenGL settings
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (recorder.sample) {
			printf("%f, %f\n", recorder.sample->total, recorder.sample->sum);
		}

		quad.Draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return EXIT_SUCCESS;
}
