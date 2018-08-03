#pragma once

typedef unsigned int GLuint;

/**
 * Renders a Fullscreen Quad rendering 
 */
class FullscreenQuad
{
private:
	GLuint vao, vbo[1];

public:
	FullscreenQuad();
	virtual ~FullscreenQuad();

	void Draw();
};

