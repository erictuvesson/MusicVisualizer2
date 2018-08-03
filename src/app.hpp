#pragma once

#include <memory>

#include "config.hpp"
#include "audio_recorder.hpp"
#include "fullscreen_quad.hpp"
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
	void draw();

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	AudioRecorder audioRecorder;
	FullscreenQuad fullscreenQuad;
	ShaderResource shader;
};
