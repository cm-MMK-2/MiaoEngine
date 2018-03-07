#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <map>
#include <vector>
#include <memory>
#include "Collider/MCollider.hpp"
class MCollisionCalculator
{
public:
	MRectCollider* AddRectCollider(float x, float y, float width, float height, CollisionCallbackFunc onCollisionEnter = nullptr, CollisionCallbackFunc onCollisionExit = nullptr)
	{
		unsigned long id = InterlockedIncrement(&indexCounter);
		colliders[id] = std::make_unique<MRectCollider>(id, x, y, width, height, onCollisionEnter, onCollisionExit);
		return (MRectCollider*)colliders[id].get();
	}
	MCircleCollider* AddCircleCollider(float x, float y, float radius, CollisionCallbackFunc onCollisionEnter = nullptr, CollisionCallbackFunc onCollisionExit = nullptr)
	{
		unsigned long id = InterlockedIncrement(&indexCounter);
		colliders[id] = std::make_unique<MCircleCollider>(id, x, y, radius, onCollisionEnter, onCollisionExit);
		return (MCircleCollider*)colliders[id].get();
	}
	void Update(double dt)
	{
		std::vector<MCollider*> temp;
		for (auto & col : colliders)
		{
			if (col.second->active)
			{
				temp.push_back(col.second.get());
			}
		}

		while (!temp.empty())
		{
			MCollider* last = temp.back();
			temp.pop_back();
			for (auto & col : temp)
			{
				if (CalculateCollision(col, last))
				{
					if (last->other == nullptr || last->other != col)
					{
						last->other = col;
						last->onCollisionEnter(last);
						col->onCollisionEnter(col);
					}
				}
			}
		}
		
	}
private:
	bool CalculateCollision(MCollider* a, MCollider* b)
	{
		if (a->type == ColliderType::Rect && b->type == ColliderType::Rect)
		{

		}
		else if (a->type == ColliderType::Rect && b->type == ColliderType::Circle)
		{

		}
		else if (a->type == ColliderType::Circle && b->type == ColliderType::Rect)
		{

		}
		else if (a->type == ColliderType::Circle && b->type == ColliderType::Circle)
		{

		}
	}
	unsigned long indexCounter = 0;
	std::map<unsigned long, std::unique_ptr<MCollider>> colliders;
};