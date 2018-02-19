#pragma once
#include "Components/PhysicsNode.h"
#include "Components/VelocityNode.h"

class Entity
{
public:

	Entity(const float aggroRange, const float scaleX, const float scaleY,  const float depth = 0, const float rotation = 0.0f) 
		: physicsNode(Vector3(scaleX, scaleY, 1.0f), depth, rotation), 
		aggroRange(aggroRange),
		entitiesInRange(0) {};


	Entity(const Vector2 position, const float aggroRange, const float scaleX, const float scaleY, const float depth = 0, const float rotation = 0.0f)
		:Entity(aggroRange, scaleX, scaleY, depth, rotation){

		this->physicsNode.setPosition(position);
	}

	Entity(const int gridX, const int gridY, const float aggroRange, const float scaleX, const float scaleY, const float depth = 0, const float rotation = 0.0f)
		:Entity(aggroRange, scaleX, scaleY, depth, rotation){

		this->setGridPosition(gridX, gridY);
	}

	float getAggroRange() const { return aggroRange; }
	int getEntitiesInRange() const { return entitiesInRange; }
	
	void setAggroRange(const float aggro) { this->aggroRange = aggro; }
	void setEntititiesInRange(const int entitities) { this->entitiesInRange = entitities; }

	void setGridPosition(const int x, const int y) {
		physicsNode.setPosition(Map::gridToWorldPosition(x, y));
	}

	void setPhysics(){
		
	}

	void setVelocity(){
		
	}

	~Entity() {};
	PhysicsNode physicsNode;
	//VelocityNode velocityNode;
private:
	float aggroRange;
	int entitiesInRange;

};

