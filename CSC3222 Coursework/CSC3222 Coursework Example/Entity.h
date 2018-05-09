#pragma once
#include "Components/PhysicsNode.h"
#include "Components/VelocityNode.h"
#include "Components/CollidableNode.h"

class Entity
{
public:

	Entity(const float aggroRange, const float scaleX, const float scaleY, const float maxSpeed, const float mass = 1.0f, const float depth = 0, const float rotation = 0.0f) 
		: physicsNode(Vector3(scaleX, scaleY, 1.0f), depth, rotation), 
		velocityNode(&physicsNode, maxSpeed, mass),
		aggroRange(aggroRange),
		entitiesInRange(0),
		collidableNode(){};


	Entity(const Vector2 position, const float aggroRange, const float scaleX, const float scaleY, const float maxSpeed, const float mass = 1.0f, const float depth = 0, const float rotation = 0.0f)
		:Entity(aggroRange, scaleX, scaleY, maxSpeed, mass,  depth, rotation){

		this->physicsNode.setPosition(position);
	}

	Entity(const Entity& src) : physicsNode(src.physicsNode), velocityNode(src.velocityNode), collidableNode(src.collidableNode), aggroRange(src.aggroRange), entitiesInRange(src.entitiesInRange) {
		//velocity node should point to this entities physics node
		velocityNode.setPhysicsNode(&physicsNode);
	}

	//Entity(const int gridX, const int gridY, const float aggroRange, const float scaleX, const float scaleY, const float maxSpeed, const float mass = 1.0f, const float depth = 0, const float rotation = 0.0f)
	//	:Entity(aggroRange, scaleX, scaleY, maxSpeed, mass, depth, rotation){

	//	this->setGridPosition(gridX, gridY);
	//}

	float getAggroRange() const { return aggroRange; }
	int getEntitiesInRange() const { return entitiesInRange; }
	
	void setAggroRange(const float aggro) { this->aggroRange = aggro; }
	void setEntititiesInRange(const int entitities) { this->entitiesInRange = entitities; }

	void setGridPosition(const int x, const int y) {
		physicsNode.setPosition(Map::gridToWorldPosition(x, y));
	}


	void lookAt(Vector2 target){
		Vector2 dir = target - physicsNode.getPosition();
		dir.Normalise();
		physicsNode.setRotation(dir);
	}

	void setColour(Vector4 colour){
		physicsNode.colour = colour;
	}

	void setPhysics(){
		
	}

	void setVelocity(){
		
	}


	virtual void update(float msec){
		velocityNode.update(msec);
	}


	Entity& operator=(const Entity &e){

		// check for self-assignment
		if (&e == this)
			return *this;

		physicsNode = e.physicsNode;
		collidableNode = e.collidableNode;

		//we want the pointer to point to the physics node belonging to this entity
		//could also just not change the pointer but this makes it more obvious.
		velocityNode.setPhysicsNode(&physicsNode);
		aggroRange = e.aggroRange;
		entitiesInRange = e.entitiesInRange;
		
		return *this;
	}

	PhysicsNode physicsNode;

	//make these pointers
	//do not update if null pointer
	//this way we have a horrible hacky pluggable component system yay
	VelocityNode velocityNode;
	CollidableNode collidableNode;

	//use to add/remove "components"
	void setVelocityNode();

	void removeVelocityNode();

	void setCollidableNode(float boundingRadius, bool fixed = true);

	void removeCollidableNode();


	void setTile(Vector2 origin, TileType terrain) {
		tileOrigin = origin;
		terrainType = terrain;
	};

	void setTerrainType(TileType terrain) {
		terrainType = terrain;
	}




	//we will maybe get around to using this eventually
	//vector<Entity> children;
protected:
	float feetToPixels(float feet) {
		return (feet  * Map::GRID_SIZE) / 5;
	}

	TileType terrainType;
	Vector2 tileOrigin;

private:
	float aggroRange;
	int entitiesInRange;

};

