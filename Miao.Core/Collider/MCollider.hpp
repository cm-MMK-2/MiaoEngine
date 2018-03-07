#pragma once
enum class ColliderType
{
	Undefined,
	Rect,
	Circle
};
typedef void(*CollisionCallbackFunc)(MCollider*);
class MCollider
{
public:
	MCollider(unsigned long _id, float _x, float _y, CollisionCallbackFunc collisionEnterCallback = nullptr, CollisionCallbackFunc collisionExitCallback = nullptr)
		: id(_id), x(_x), y(_y), onCollisionEnter(collisionEnterCallback), onCollisionExit(collisionExitCallback){}
	void CollisionEnter(MCollider* other) {
		if (onCollisionEnter != nullptr)
		{
			onCollisionEnter(other);
		}
	}
	void CollisionExit(MCollider* other) {
		if (onCollisionExit != nullptr)
		{
			onCollisionExit(other);
		}
	}
	CollisionCallbackFunc onCollisionEnter;
	CollisionCallbackFunc onCollisionExit;
	unsigned long id;
	ColliderType type = ColliderType::Undefined;
	bool active = true;
	float x;
	float y;
	MCollider* other = nullptr;
};

class MRectCollider : public MCollider
{
public:
	MRectCollider(unsigned long _id, float _x, float _y, float _width, float _height, CollisionCallbackFunc collisionEnterCallback = nullptr, CollisionCallbackFunc collisionExitCallback = nullptr)
		:MCollider(_id, _x, _y, collisionEnterCallback, collisionExitCallback), width(_width), height(_height)
	{
		type = ColliderType::Rect;
	}
	float width;
	float height;
};

class MCircleCollider : public MCollider
{
public:
	MCircleCollider(unsigned long _id, float _x, float _y,float _radius, CollisionCallbackFunc collisionEnterCallback = nullptr, CollisionCallbackFunc collisionExitCallback = nullptr)
		:MCollider(_id, _x, _y, collisionEnterCallback, collisionExitCallback), radius(_radius)
	{
		type = ColliderType::Circle;
	}
	float radius;
};