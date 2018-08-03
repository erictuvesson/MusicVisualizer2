#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <memory>

#include "audio_recorder.hpp"
#include "fullscreen_quad.hpp"
#include "shader.hpp"

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
	Shader shader;
};
