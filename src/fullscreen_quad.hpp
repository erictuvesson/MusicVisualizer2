#pragma once

#include "config.hpp"

/**
 * Renders a Fullscreen Quad rendering 
 */
class FullscreenQuad
{
public:
	FullscreenQuad();
	virtual ~FullscreenQuad();

	void Initialize();
	void Draw();

private:
	GLuint vao, vbo[1];
};
