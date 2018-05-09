#pragma once
#include "../Entity.h"
#include "../MovementManager.h"
const float DRAGON_SPEED_FEET = 3;
class Dragon : public Entity{
public:
	Dragon(const float rotation = 0.0f) : Entity(25.0f, 50.0f, 50.0f, 1000.0f, 1.0f, -199.0f, rotation), movementManager(velocityNode, collidableNode, feetToPixels(100), feetToPixels(50)){
		physicsNode.setBoundingCircle(50.0f);
		collidableNode.boundingRadius = 50.0f;
		collidableNode.isFixed = false;
	}

	Dragon(const Vector2 pos, const float rotation = 0.0f) : Dragon(rotation){
		this->physicsNode.setPosition(pos);
	}
	Dragon(const int gridX, const int gridY, const float rotation = 0.0f) : Dragon(rotation){
		this->setGridPosition(gridX, gridY);
	};

	void update(float msec) override;
	void setObstacles(std::vector<Entity>* obstacle);

	Vector2 targetLocation;
	bool moveToTarget;
protected:
	MovementManager movementManager;
};

