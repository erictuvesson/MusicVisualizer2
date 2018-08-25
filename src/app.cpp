#include "app.hpp"
#include "ImGuiHelper.hpp"

#include <iostream>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	800

App::App()
	: window(nullptr)
	, glContext(nullptr)
	, audioSumQueue(new Graph(200))
	, audioTimeQueue(new Graph(200))
	, audioTimeHQueue(new Graph(200))
	, audioTimeLQueue(new Graph(200))
	, curve(new Curve())
{
	scenes.push_back("animation_2d_polygon_scene.glsl");
	scenes.push_back("animation_2d_heart_scene.glsl");
	scenes.push_back("animation_2d_line_scene.glsl");
	scenes.push_back("animation_2d_animation_scene.glsl");
	scenes.push_back("basic_color_scene.glsl");
	scenes.push_back("raycasting_scene.glsl");
	scenes.push_back("raymarching_scene.glsl");
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

	// Setup ImGui binding
	ImGui::Initialize(window, glContext);

	// Initialize the audio device
	//updateAudioDevices();
	//for (int i = 0; i < audioDevices.size(); i++) {
	//	std::cout << i << ". " << audioDevices[i].info.name << '\n';
	//}
	// 
	// this->selectedAudioDevice = std::cin.get() - '0';

	setAudioDevice();

	fullscreenQuad.Initialize();

	if (!setScene()) {
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
		for (int i = 0; i < 256; i++) {
			shaderState.iSample[i] = sample->fft[i];
		}

		shaderState.iAudioSum = sample->sum;

		audioLastTime = audioTime;
		audioTime += sample->sum;
		lastAudioSum = sample->sum;

		const float audioH = curve->At(audioTime - audioLastTime, 0.5f) * 0.025f;
		audioTimeHQueue->Push(audioH);

		//const float audioL = curve->At(sample->sum, 0.1f) * 0.0025f;
		const float audioL = sample->sum > 0.01f ? 0.004f : 0.0f;
		audioTimeLQueue->Push(audioL);

		const float audioTime = (audioH + (audioL - audioH) * audioLHMix) * audioIntensity;
		shaderState.iAudioTime += audioTime;
		audioTimeQueue->Push(audioTime);
	} else {
		shaderState.iAudioSum = 0;
	}

	audioSumQueue->Push(shaderState.iAudioSum);

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
	if (ImGui::BeginSimple("info"))
	{
		ImGui::Text("Frame Time: %.3f ms (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Separator();

		ImGui::Text("Audio Input");
		ImGui::PlotLines("", audioSumQueue->Data(), audioSumQueue->Size(), 0, NULL, 0, 5, ImVec2(0, 100), 4);
		ImGui::Text("Max: %.2f, Average: %.2f", audioSumQueue->Max(), audioSumQueue->Average());
		ImGui::Separator();

		ImGui::Text("Audio Input FFT");
		ImGui::PlotLines("", &shaderState.iSample[0], 256, 0, NULL, 0, 0.5f, ImVec2(0, 100), 4);

		if (ImGui::CollapsingHeader("Audio Time")) {
			ImGui::Text("Audio high");
			ImGui::PlotLines("", audioTimeHQueue->Data(), audioTimeHQueue->Size(), 0, NULL, 0, 0.05f, ImVec2(0, 50), 4);

			ImGui::Text("Audio low");
			ImGui::PlotLines("", audioTimeLQueue->Data(), audioTimeLQueue->Size(), 0, NULL, 0, 0.01f, ImVec2(0, 50), 4);

			ImGui::Text("Audio Final");
			ImGui::PlotLines("", audioTimeQueue->Data(), audioTimeQueue->Size(), 0, NULL, 0, 0.05f, ImVec2(0, 50), 4);
		}
	}
	ImGui::End();

	if (ImGui::BeginSimple("settings", NULL, 1))
	{
		if (ImGui::Combo("Scene", &selectedScene, scenes.data(), (int)scenes.size())) {
			setScene();
		}

		if (ImGui::Combo("Device", &selectedAudioDevice, audioDevicesNames.data(), (int)audioDevicesNames.size())) {
			setAudioDevice();
		}

		ImGui::Separator();

		ImGui::SliderFloat("Intensity", &audioIntensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Mix L/H", &audioLHMix, 0.0f, 1.0f);
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

bool App::setScene()
{
	try {
		auto tmp = ShaderFactory::CompileShader({ "shaders/" + std::string(scenes[selectedScene]) });
		if (tmp == nullptr) {
			return false;
		}

		this->shader = std::move(tmp);
	}
	catch (const std::exception& e) {
		std::cout << "-- Switch scene failed" << '\n';
		std::cout << "Exception: " << e.what() << '\n';
		return false;
	}
	return true;
}
