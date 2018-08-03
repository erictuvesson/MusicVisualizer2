#pragma once

typedef unsigned int GLuint;

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

