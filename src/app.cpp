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


	{
		int wx, wy;
		SDL_GetWindowSize(window, &wx, &wy);
		shaderState.iResolution[0] = wx;
		shaderState.iResolution[1] = wy;
		shaderState.iResolution[2] = 0;

	}



	bool isRunning = true;
	unsigned int lastTime = 0, currentTime = 0;
	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}

		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();

		const float deltaTime = (currentTime - lastTime) * 1000 / SDL_GetPerformanceFrequency();

		draw(deltaTime / 1000);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	audioRecorder.Destroy();

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
	window = SDL_CreateWindow("Music Visualizer",
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

	shader = ShaderFactory::CompileShader({ "tutorial6.frag" });
	if (shader == nullptr) {
		std::cin.get();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void App::draw(float elapsedtime)
{
	shaderState.iTime += elapsedtime;
	shaderState.iTimeDelta = elapsedtime;
	shaderState.iFrame++;

	if (auto sample = audioRecorder.GetSample()) {
		shaderState.iAudioSum = sample->sum;
		shaderState.iAudioAverage = sample->average;
	}
	else {
		shaderState.iAudioSum = 0;
		shaderState.iAudioAverage = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	shader->Apply(shaderState);
	fullscreenQuad.Draw();

	SDL_GL_SwapWindow(window);
}
