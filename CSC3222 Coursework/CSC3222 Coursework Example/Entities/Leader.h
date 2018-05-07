#pragma once
#include "../Entity.h"
#include "Follower.h"
struct LeaderController{
	bool rotateLeft;
	bool rotateRight;
	bool moveForward;
	//const float rotateSpeed = 0.01f;
};

class Leader :
	public Follower {
public:
	Leader(const Vector2 pos = Vector2(0.0f, 0.0f), const float rotation = 0.0f) : Follower(pos, rotation), leaderControler({ false, false, false }) {
		velocityNode.setMaxSpeed(1000.0f);
		physicsNode.setDepth(-199.5f);
	};
	Leader(const int gridX, const int gridY, const float rotation = 0.0f) : Follower(gridX, gridY, rotation), leaderControler({ false, false, false }) {
		velocityNode.setMaxSpeed(1000.0f);
		physicsNode.setDepth(-199.5f);
	};

	void update(float msec);

	LeaderController leaderControler;
};

