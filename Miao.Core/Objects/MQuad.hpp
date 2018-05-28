#pragma once
#include "MObject.hpp"

class MQuad : public MObject
{
public:
	MRect rect;
	virtual void Draw(double dt) = 0;
	static void CreateQuadVAO()
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(basic_quad_vertices), basic_quad_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	static void ReleaseQuadVAO()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
protected:
	static GLuint GetVAO() {
		return vao;
	}
private:
	static GLuint vbo;
	static GLuint vao;
	const static GLfloat basic_quad_vertices[16];
};

const GLfloat MQuad::basic_quad_vertices[] = {
	// Position // Texcoords
	0.0f, 0.0f, 0.0f, 0.0f, //bottom-left
	0.0f, 1.0f, 0.0f, 1.0f, //top-left
	1.0f, 0.0f, 1.0f, 0.0f, //bottom-right
	1.0f, 1.0f, 1.0f, 1.0f //top-right
};
GLuint MQuad::vao;
GLuint MQuad::vbo;