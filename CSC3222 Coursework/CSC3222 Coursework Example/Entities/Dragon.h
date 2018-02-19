#pragma once
#include "../Entity.h"
class Dragon :
	public Entity
{
public:
	Dragon(const Vector2 pos = Vector2(0.0f, 0.0f), const float rotation = 0.0f) : Entity(25.0f, pos, -199.5f, rotation, 50.0f, 50.0f) {}
	Dragon(const int gridX, const int gridY, const float rotation = 0.0f) : Entity(25.0f, gridX, gridY, -199.5f, rotation, 50.0f, 50.0f) {};
	~Dragon() {}
};

