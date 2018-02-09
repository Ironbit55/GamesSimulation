#pragma once
#include "PhysicsNode.h"
class Entity
{
public:


	Entity() : physicsNode() {};
	Entity(const float aggroRange, const Vector2 position = Vector2(0.0f, 0.0f), const float depth = 0, const float rotation = 0.0f,
		const float scaleX = 1.0f, const float scaleY = 1.0f)
		: physicsNode(position, depth, rotation, Vector3(scaleX, scaleY, 1.0f)) {};

	Entity(const float aggroRange, const int gridX, const int gridY, const float rotation = 0.0f,
		const float scaleX = 1.0f, const float scaleY = 1.0f)
		: physicsNode(gridX, gridY, rotation, Vector3(scaleX, scaleY, 1.0f)) {};

	float const getAggroRange() { return aggroRange; }
	int const getEntitiesInRange() { return entitiesInRange; }
	
	void setAggroRange(float aggro) { this->aggroRange = aggro; }
	void setEntititiesInRange(int entitities) { this->entitiesInRange = entitities; }

	~Entity() {};
	PhysicsNode physicsNode;
private:
	float aggroRange;
	int entitiesInRange;

};

