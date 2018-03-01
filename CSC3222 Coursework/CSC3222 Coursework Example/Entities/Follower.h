#pragma once
#include "../Entity.h"
class Follower :
	public Entity
{
public:
	Follower(const float rotation = 0.0f) : Entity(0.0f, 10.0f, 10.0f, 5.0f, 1.0f, -199.9f, rotation){
		physicsNode.setBoundingCircle(4.0f);
	};
	Follower(const Vector2 pos, const float rotation = 0.0f) : Follower(rotation){
		physicsNode.setPosition(pos);
	};
	Follower(const int gridX, const int gridY, const float rotation = 0.0f) : Follower(rotation){
		this->setGridPosition(gridX, gridY);
	};
	virtual void update(float msec);

	Vector2 leaderLocation;
	bool followLeader;
};

