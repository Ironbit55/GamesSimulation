#pragma once
#include "../Entity.h"
class Dragon : public Entity{
public:
	Dragon(const float rotation = 0.0f) : Entity(25.0f, 50.0f, 50.0f, 5.0f, 1.0f, -199.5f, rotation){
		physicsNode.setBoundingCircle(30.0f);
	}

	Dragon(const Vector2 pos, const float rotation = 0.0f) : Dragon(rotation){
		this->physicsNode.setPosition(pos);
	}
	Dragon(const int gridX, const int gridY, const float rotation = 0.0f) : Dragon(rotation){
		this->setGridPosition(gridX, gridY);
	};

	void update(float msec) override;

	Vector2 targetLocation;
	bool moveToTarget;
};

