#pragma once
#include "MImage.hpp"

enum MButtonState {
	Normal = 0,
	Hover = 1,
	Down = 2,
	Disabled = -1
};


template <typename T>
class MButton :public MImage
{
public:
	using ButtonClickCallback = void(*)(int, T);
	MButton(Texture2D* _texture, Rect _rect, GLfloat _rotation, glm::vec3 _color, Shader* _shader, unsigned long _id, ButtonClickCallback _clickCallback = nullptr) :MImage(_texture, _rect, _rotation, _color, _shader, _id)
	{
		state = 0;
		hitArea = Rect(0, 0, _rect.w, _rect.h);
		OnClick = _clickCallback;
	}
	Rect hitArea;//Shape hitArea
	MButtonState state;
	ButtonClickCallback OnClick;
	void Click(int type = 0, T args = NULL)
	{
		state = 2;
		if (nullptr != OnClick)
		{
			this.OnClick();
		}
	}
};