#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Data/texture.hpp"
#include "Data/shader.hpp"
#include <memory>
#include <vector>
#include "Objects/MObject.hpp"
#include "Objects/MQuad.hpp"
#include "Objects/MImage.hpp"
#include "Objects/MText.hpp"
#include "Objects/MSpriteSheetAnimation.hpp"
#include "Objects/MSpineObject.hpp"

/*
 *     ∧ ∧
 *　　( ﾟωﾟ)
 * ＿(_つ/￣￣￣/＿
 * 　 ＼/　　　/
 *	　　￣￣￣
 * render objects in queue
 */
class MRenderQueue
{
public:
	void Init()
	{
		//initialize MQuad
		MQuad::CreateQuadVAO();
	}

	void Release()
	{
		MQuad::ReleaseQuadVAO();
	}

	void Render(double dt)
	{
		for (auto & e : objects)
		{
			if (e.second->visible)
			{
				e.second->Draw(dt);
			}
		}
	}

	bool Remove(unsigned long id)
	{
		objects.erase(id);
	}

	//add into map
	MImage * CreateImage(Texture2D* texture, Rect rect, GLfloat rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f), Shader* shader = nullptr)
	{
		if (!texture)
		{
			//ERROR
		}
		unsigned long id = InterlockedIncrement(&indexCounter);
		objects[id] = std::make_unique<MImage>(texture, rect, rotation, color, shader, id);
		return static_cast<MImage*>(objects[id].get());
	}

	MText * CreateText(FT_Face face, std::wstring text, Rect rect, int size, Vec3 color, Shader* shader = nullptr, int option = GL_STATIC_DRAW)
	{
		unsigned long id = InterlockedIncrement(&indexCounter);
		objects[id] = std::make_unique<MText>(face, text, rect, size, color, shader, option, id);
		return static_cast<MText*>(objects[id].get());
	}

	MSpriteSheetAnimation * CreateSpriteSheetAnimation(Texture2D* texture, Rect rect, Vec2 block, GLfloat rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f), Shader* shader = nullptr)
	{
		if (!texture)
		{
			//ERROR
		}
		unsigned long id = InterlockedIncrement(&indexCounter);
		objects[id] = std::make_unique<MSpriteSheetAnimation>(texture, rect, block, rotation, color, shader, id);
		return static_cast<MSpriteSheetAnimation*>(objects[id].get());
	}

	MSpineObject * CreateSpineObject(SpineData& spineData, Shader* shader, Vec2 position, Vec2 scale)
	{
		unsigned long id = InterlockedIncrement(&indexCounter);
		objects[id] = std::make_unique<MSpineObject>(spineData, shader, position, scale, id);
		return static_cast<MSpineObject*>(objects[id].get());
	}

private:
	unsigned long indexCounter = 0;
	std::map<unsigned long,std::unique_ptr<MObject>> objects;
};