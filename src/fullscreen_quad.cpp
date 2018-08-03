#include "fullscreen_quad.hpp"

FullscreenQuad::FullscreenQuad()
{

}

FullscreenQuad::~FullscreenQuad()
{
	// glDisableVertexAttribArray(0);
	// glDeleteBuffers(1, vbo);
	// glDeleteVertexArrays(1, &vao);
}

void FullscreenQuad::Initialize()
{
	const float quad[6][2] = {
		{ -1.0f, -1.0f },
		{ -1.0f, +1.0f },
		{ +1.0f, +1.0f },
		{ -1.0f, -1.0f },
		{ +1.0f, -1.0f },
		{ +1.0f, +1.0f },
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void FullscreenQuad::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
