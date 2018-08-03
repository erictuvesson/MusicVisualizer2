#include "app.hpp"

#include <iostream>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	800

App::App()
	: window(nullptr)
	, glContext(nullptr)
{

}

App::~App()
{

}

int App::Run()
{
	const int error = initialize();
	if (error != EXIT_SUCCESS) {
		return error;
	}

	bool isRunning = true;
	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		draw();
	}

	SDL_GL_DeleteContext(glContext);
	SDL_Quit();

	return EXIT_SUCCESS;
}

int App::initialize()
{
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

	if (window == nullptr) {
		std::cout << "Failed to create SDL Window";
		return EXIT_FAILURE;
	}

	// Create the OpenGL context for the window using SDL
	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		std::cout << "Failed to create OpenGL Context";
		return EXIT_FAILURE;
	}

	// Enable glew experimental, this enables some more OpenGL extensions.
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize glew";
		return EXIT_FAILURE;
	}

	// Initialize the audio device
	auto devices = audioRecorder.GetDevices();
	audioRecorder.Listen(devices[devices.size() - 1]);

	fullscreenQuad.Initialize();

	// Initialize the shader
	if (!shader.Compile("tutorial2.vert", "tutorial2.frag")) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void App::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (auto sample = audioRecorder.GetSample()) {
		printf("%f, %f\n", sample->sum, sample->average);
	}

	shader.Apply();
	fullscreenQuad.Draw();

	SDL_GL_SwapWindow(window);
}
