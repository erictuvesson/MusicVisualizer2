#include "FullscreenQuad.hpp"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

FullscreenQuad::FullscreenQuad()
{
	const float quad[6][2] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },

		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f }
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

FullscreenQuad::~FullscreenQuad()
{
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, &vao);
}

void FullscreenQuad::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 12);
}
