#pragma once
#include "MImage.hpp"

class MSpriteSheetAnimation :public MImage
{
public:
	MSpriteSheetAnimation(const MSpriteSheetAnimation & copy) = delete;
	MSpriteSheetAnimation(Texture2D* _texture, Rect _rect, Vec2 _block, GLfloat _rotation, glm::vec3 _color, Shader* _shader, unsigned long _id):MImage(_texture, _rect, _rotation, _color, _shader, _id)
	{
		id = _id;
		texture = *_texture;
		shader = *_shader;
		rect = _rect;
		block = _block;
		rotation = _rotation;
		color = _color;
		blockSize.x = (int)(1 / _block.x);
		blockSize.y = (int)(1 / _block.y);
		const float vertices[] = {
			// Position // Texcoords
			0.0f, 0.0f, 0.0f, 0.0f, //bottom-left
			0.0f, 1.0f, 0.0f, _block.y, //top-left
			1.0f, 0.0f, _block.x, 0.0f, //bottom-right
			1.0f, 1.0f, _block.x, _block.y //top-right
		};
		GLuint vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	~MSpriteSheetAnimation()
	{
		glDeleteVertexArrays(1, &vao);
	}
	//void Destroy()
	//{

	//}
	inline virtual void Draw(double dt)
	{

		pTime += dt;
		
		while (pTime > perFrame)
		{
			pTime -= perFrame;
			if (frame < frameTotal)
			{
				++frame;
			}
			else
			{
				frame = 0;
			}
		}
		
		//Prepare transformations
		shader.UseProgram();
		glm::mat4 model;
		model = glm::translate(model, Vec3(rect.x, rect.y, 0.0f));

		model = glm::translate(model, Vec3(0.5f * rect.w, 0.5f * rect.h, 0.0f));
		model = glm::rotate(model, rotation, Vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, Vec3(-0.5f * rect.w, -0.5f * rect.h, 0.0f));

		model = glm::scale(model, Vec3(rect.w, rect.h, 1.0f));
		Vec2 uv; //current position
		uv.x = block.x * (frame / blockSize.x);
		uv.y = block.y * (frame % blockSize.x);
		shader.SetVector2f("uv", uv);
		shader.SetMatrix4("model", model);

		// Render textured quad
		shader.SetVector3f("mainColor", color);
		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		int error = glGetError();
	}
private:
	double pTime = 0;
	uint32_t frame;//current frame index
	uint32_t frameTotal = 20;
	Vec2 block;
	float perFrame = 0.2f;
	Vecotr2<int> blockSize; //x * y blocks
	GLuint vao;
};