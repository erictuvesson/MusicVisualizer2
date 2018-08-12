#pragma once

#include <memory>
#include <vector>

#include "config.hpp"
#include "audio_recorder.hpp"
#include "fullscreen_quad.hpp"
#include "shader_factory.hpp"
#include "shader_resource.hpp"

/**
 * App, the music visualizer!
 */
class App
{
public:
	App();
	virtual ~App();

	/**
	 * Start the application.
	 */
	int Run();

private:
	int initialize();
	void draw(float elapsedtime);
	void drawDebug();

	void setAudioDevice();
	void updateAudioDevices();

private:
	// Graphics
	SDL_Window* window;
	SDL_GLContext glContext;
	FullscreenQuad fullscreenQuad;
	ShaderState shaderState;
	std::unique_ptr<ShaderResource> shader;
	
	// Audio
	AudioRecorder audioRecorder;
	std::vector<AudioDevice> audioDevices;
	std::vector<const char*> audioDevicesNames;
	int selectedAudioDevice = 0;

	// Data
	std::vector<float> audioSumQueue;
};
