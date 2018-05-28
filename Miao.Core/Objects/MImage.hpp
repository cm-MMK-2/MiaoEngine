#pragma once
#include "MQuad.hpp"

class MImage :public MQuad
{
public:
	MImage(const MImage & copy) = delete;
	MImage(Texture2D* _texture, MRect _rect, GLfloat _rotation, glm::vec3 _color, Shader* _shader, unsigned long _id)
	{
		id = _id;
		texture = *_texture;
		shader = *_shader;
		rect = _rect;
		rotation = _rotation;
		color = _color;
	}
	~MImage()
	{
	}
	//void Destroy()
	//{

	//}
	virtual void Draw(double dt)
	{
		//Prepare transformations
		shader.UseProgram();
		glm::mat4 model;
		model = glm::translate(model, Vec3(rect.x, rect.y , 0.0f));

		model = glm::translate(model, Vec3(0.5f * rect.w, 0.5f * rect.h, 0.0f));
		model = glm::rotate(model, rotation, Vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, Vec3(-0.5f * rect.w, -0.5f * rect.h, 0.0f));

		model = glm::scale(model, Vec3(rect.w, rect.h, 1.0f));

		shader.SetMatrix4("model", model);
		// Render textured quad
		shader.SetVector3f("mainColor", color);
		glActiveTexture(GL_TEXTURE0);
		texture.Bind();
		glBindVertexArray(GetVAO());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	Shader shader;
	Texture2D texture;
	GLfloat rotation;
	Vec3 color;
};