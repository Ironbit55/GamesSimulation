#pragma once
#include "../Entity.h"
class Follower :
	public Entity
{
public:
	Follower(const Vector2 pos = Vector2(0.0f, 0.0f), const float rotation = 0.0f) : Entity(0.0f, pos, -199.9f, rotation, 10.0f, 10.0f) {};
	Follower(const int gridX, const int gridY, const float rotation = 0.0f) : Entity(0.0f, gridX, gridY, -199.9f, rotation, 10.0f, 10.0f) {};
	~Follower() {}
};

