#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Data/texture.hpp"
#include "../Data/shader.hpp"

typedef glm::vec3 Vec3;
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;

template <typename T>
struct Vecotr2
{
	T x;
	T y;
};

struct Rect
{
	float x;
	float y;
	float w;
	float h;
	Rect()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	Rect(Rect & _rect)
	{
		x = _rect.x;
		y = _rect.y;
		w = _rect.w;
		h = _rect.h;
	}
	Rect(float _x, float _y, float _w, float _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}
};

class MObject
{
public:
	virtual void Draw(double dt) = 0;
	void Hide()
	{
		visible = false;
	}
	void Show()
	{
		visible = true;
	}
	void Destroy() {

	}
	bool visible = true;
	unsigned long id;
};