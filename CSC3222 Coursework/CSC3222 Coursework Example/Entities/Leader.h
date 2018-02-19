#pragma once
#include "../Entity.h"
#include "Follower.h"

class Leader :
	public Follower
{
public:
	Leader(const Vector2 pos = Vector2(0.0f, 0.0f), const float rotation = 0.0f) : Follower(pos, rotation) {};
	Leader(const int gridX, const int gridY, const float rotation = 0.0f) : Follower(gridX, gridY, rotation) {};
	~Leader() {}
};

