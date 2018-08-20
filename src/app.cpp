#include "app.hpp"

#include <iostream>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	800

App::App()
	: window(nullptr)
	, glContext(nullptr)
	, audioSumQueue(new Graph(200))
	, audioTimeDeltaQueue(new Graph(200))
	, curve(new Curve())
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
		shaderState.iResolution[0] = (float)wx;
		shaderState.iResolution[1] = (float)wy;
		shaderState.iResolution[2] = 0.0f;

	}



	bool isRunning = true;
	unsigned int lastTime = 0, currentTime = 0;
	SDL_Event event;
	while (isRunning) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0) {
					this->showDebug = !showDebug;
				}
				if (event.key.keysym.sym == SDLK_F11 && event.key.repeat == 0) {
					Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
					bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag; 
					SDL_SetWindowFullscreen(window, IsFullscreen ? 0 : FullscreenFlag);
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		lastTime = currentTime;
		currentTime = (unsigned int)SDL_GetPerformanceCounter();

		const float deltaTime = (float)(
			(currentTime - lastTime) * 1000 / (unsigned int)SDL_GetPerformanceFrequency());

		draw(deltaTime / 1000);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup style
	ImGui::StyleColorsDark();

	// Initialize the audio device
	//updateAudioDevices();
	//for (int i = 0; i < audioDevices.size(); i++) {
	//	std::cout << i << ". " << audioDevices[i].info.name << '\n';
	//}
	// 
	// this->selectedAudioDevice = std::cin.get() - '0';

	setAudioDevice();

	fullscreenQuad.Initialize();

	shader = ShaderFactory::CompileShader({ "shaders/animation_2d_heart_scene.glsl" });
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

		audioLastTime = audioTime;
		audioTime += sample->sum;
		audioTimeDelta = audioTime - audioLastTime;

		if (lastAudioSum < sample->sum) {
			audioTimeDelta *= 0.25f;
			std::cout << audioTimeDelta << '\n';
		}

		lastAudioSum = sample->sum;

		shaderState.iAudioTime += curve->At(audioTimeDelta);
	}
	else {
		shaderState.iAudioSum = 0;
	}

	audioSumQueue->Push(shaderState.iAudioSum);
	audioTimeDeltaQueue->Push(audioTimeDelta);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Apply(shaderState);
	fullscreenQuad.Draw();

	if (showDebug) {
		drawDebug();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window);
}

void App::drawDebug()
{
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);

	if (corner != -1)
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

	ImGui::SetNextWindowBgAlpha(1.0f);
	if (ImGui::Begin("", NULL, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("%.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::PlotLines("", audioSumQueue->Data(), audioSumQueue->Size(), 0, NULL, 0, 3, ImVec2(0, 100), 4);
		ImGui::PlotLines("", audioTimeDeltaQueue->Data(), audioTimeDeltaQueue->Size(), 0, NULL, 0, 0.1f, ImVec2(0, 100), 4);
		ImGui::BeginChild("AudioInfo", ImVec2(0, 50));
		{
			ImGui::Text("Audio: %f", shaderState.iAudioSum);
			ImGui::Text("Audio Time: %f", audioTimeDelta);
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Settings", ImVec2(0, 50));
		{
			if (ImGui::Combo("Device", &selectedAudioDevice, audioDevicesNames.data(), (int)audioDevicesNames.size())) {
				setAudioDevice();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void App::setAudioDevice()
{
	updateAudioDevices();
	audioRecorder.Listen(audioDevices[selectedAudioDevice]);
}

void App::updateAudioDevices()
{
	this->audioDevices = audioRecorder.GetDevices();

	audioDevicesNames.clear();
	for (auto device : audioDevices) {
		audioDevicesNames.push_back(device.info.name);
	}
}
