#pragma once

#include <memory>
#include <vector>

#include "config.hpp"
#include "audio_recorder.hpp"
#include "fullscreen_quad.hpp"
#include "shader_factory.hpp"
#include "shader_resource.hpp"
#include "graph.hpp"
#include "curve.hpp"

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

	bool setScene();

private:
	// Graphics
	SDL_Window* window;
	SDL_GLContext glContext;
	FullscreenQuad fullscreenQuad;
	ShaderState shaderState;
	std::unique_ptr<ShaderResource> shader;

	// System
	std::vector<const char*> scenes;
	int selectedScene = 0;
	bool showDebug = true;
	float audioIntensity = 1.0f;
	float audioLHMix = 0.5f;

	// Audio
	AudioRecorder audioRecorder;
	std::vector<AudioDevice> audioDevices;
	std::vector<const char*> audioDevicesNames;
	int selectedAudioDevice = 0;
	float audioTime = 0, audioLastTime = 0, audioTimeDelta = 0;
	float lastAudioSum = 0;

	// Data
	std::unique_ptr<Graph> audioSumQueue;
	std::unique_ptr<Graph> audioTimeQueue, audioTimeHQueue, audioTimeLQueue;
	std::unique_ptr<Curve> curve;
};
