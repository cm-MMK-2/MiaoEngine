#pragma once
#include <string>
#include "MQuad.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <cmath>
#include <exception>

class MText :public MQuad
{
public:
	MText()
	{
	}
	MText(FT_Face _face, std::wstring _text, Rect _rect, unsigned int _size, Vec3 _color, Shader* _shader, int _option, unsigned long _id)
	{
		id = _id;
		face = _face;
		shader = *_shader;
		rect = _rect;
		size = _size;
		color = _color;
		text = _text;
		option = _option;
		Initialize();
	}
	~MText()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texture);
	}
    inline void Draw(double dt)
	{
		shader.UseProgram();
		// Render textured quad
		glm::mat4 model;
		model = glm::translate(model, Vec3(rect.x, rect.y, 0.0f));

		//model = glm::translate(model, Vec3(0.5f * rect.w, 0.5f * rect.h, 0.0f));
		//model = glm::rotate(model, rotation, Vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, Vec3(-0.5f * rect.w, -0.5f * rect.h, 0.0f));

		model = glm::scale(model, Vec3(rect.w, rect.h, 1.0f));

		shader.SetMatrix4("model", model);

		// Render textured quad
		shader.SetVector3f("mainColor", color);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glBindVertexArray(_vao);
		glBindVertexArray(GetVAO());
		// Render Triangle
		//glDrawElements(GL_TRIANGLES, 6 * chara_count, GL_UNSIGNED_INT, indices);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//modify text
	void SetText(std::wstring _text)
	{
		text = _text;
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texture);
		Initialize();
	}

	int size;
	GLfloat rotation;
	Vec3 color;
	std::wstring text;
	FT_Face face;
	int option;
private:
	//old Initialization backup
	//void Initialize()
	//{
	//	GLuint vbo;
	//	glGenVertexArrays(1, &_vao);
	//	glGenBuffers(1, &vbo);
	//	glGenTextures(1, &texture);

	//	if (FT_Set_Char_Size(face, 0, size * 64, 96, 96))
	//	{
	//		std::cout << "ERROR::FREETYPE: Failed to set font size" << std::endl;
	//	}
	//	FT_GlyphSlot slot = face->glyph;
	//	float _x = rect.x;
	//	float _y = rect.y;
	//	FT_Size face_size = face->size;
	//	chara_count = text.size();
	//	unsigned int gw = (face_size->metrics.max_advance >> 6) * chara_count;
	//	unsigned int gh = (face_size->metrics.ascender - face_size->metrics.descender) >> 6;
	//	unsigned int gt = (face_size->metrics.ascender - abs(face_size->metrics.descender)) >> 6;
	//	glBindVertexArray(_vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//	int buffer_size = chara_count * sizeof(GLfloat[4][4]);
	//	glBufferData(GL_ARRAY_BUFFER, buffer_size, 0, option);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	glActiveTexture(GL_TEXTURE0);

	//	// Set texture options
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gw, gh, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	//	//glClearTexImage(texture, 0, GL_BGRA, GL_UNSIGNED_BYTE, &clearColor);

	//	//std::wstring::const_iterator i;
	//	//for (i = text.begin(); i != text.end(); i++)
	//	indices = new GLuint[chara_count * 6];
	//	for (UINT i = 0; i < chara_count; i++)
	//	{
	//		if (FT_Load_Char(face, text.at(i), FT_LOAD_RENDER))
	//		{
	//			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//			continue;
	//		}
	//		GLuint w = slot->bitmap.width;
	//		GLuint h = slot->bitmap.rows;
	//		GLfloat x = _x;
	//		GLfloat y = _y + h - slot->bitmap_top + gt;

	//		GLfloat x_max = x + w;
	//		GLfloat y_min = y - h;
	//		GLfloat px_min = (x - rect.x) / (GLfloat)gw;
	//		GLfloat px_max = (x - rect.x + w) / (GLfloat)gw;
	//		GLfloat py_max = h / (GLfloat)gh;
	//		GLfloat vertices[4][4] = {
	//			{ x,     y_min,   px_min, 0.0f }, //left bottom
	//			{ x,     y,       px_min, py_max }, //left top
	//			{ x_max, y_min,   px_max, 0.0f }, //right bottom

	//			{ x_max, y,       px_max, py_max }, //right top
	//			/*{ x,     y,       px_min, py_max }, //left top
	//			{ x_max, y_min,   px_max, 0.0 }*/ //right bottom
	//		};

	//		// Update content of VBO memory
	//		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(vertices), sizeof(vertices), vertices);
	//		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)(x - rect.x), 0, w, h, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
	//		_x += (slot->advance.x >> 6); // 64
	//		int offset = i * 6;
	//		int array_offset = i * 4;
	//		indices[offset] = array_offset;
	//		indices[offset + 1] = array_offset + 1;
	//		indices[offset + 2] = array_offset + 2;
	//		indices[offset + 3] = array_offset + 1;
	//		indices[offset + 4] = array_offset + 2;
	//		indices[offset + 5] = array_offset + 3;
	//	}

	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindVertexArray(0);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

	void Initialize()
	{
		glGenTextures(1, &texture);

		//if (face == nullptr)
		//{
		//	std::cout << "ERROR::FREETYPE: FontFace is null, maybe font file is not correctly loaded" << std::endl;
		//	return;
		//}

		if (FT_Set_Char_Size(face, 0, size * 64, 96, 96))
		{
			std::cout << "ERROR::FREETYPE: Failed to set font size" << std::endl;
		}
		FT_GlyphSlot slot = face->glyph;
		GLuint _x = 0;
		FT_Size face_size = face->size;
		chara_count = text.size();
		//unsigned int gh = (face_size->metrics.ascender - face_size->metrics.descender) >> 6;
		unsigned int gt = face_size->metrics.ascender >> 6;

		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, rect.w, rect.h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
		glClearTexImage(texture, 0, GL_BGRA, GL_UNSIGNED_BYTE, &clearColor);
		std::wstring::const_iterator i;
		for (i = text.begin(); i != text.end(); i++)
		{
			if (FT_Load_Char(face, *i, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			GLuint w = slot->bitmap.width;
			GLuint h = slot->bitmap.rows;
			GLuint _y = gt - slot->bitmap_top;
			glTexSubImage2D(GL_TEXTURE_2D, 0, _x, _y, w, h, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
			_x += (slot->advance.x >> 6); // 64
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	GLuint chara_count, texture;
	Shader shader;
	const static Vec3 clearColor;
};

const Vec3 MText::clearColor = Vec3(0, 0, 0);