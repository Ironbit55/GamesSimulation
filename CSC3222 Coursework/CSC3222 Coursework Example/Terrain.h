#pragma once
#include "Entity.h"

const float BOUNDING_CIRCLE = Map::GRID_SIZE * 0.5;


class Terrain : public Entity
{
public:
	
	~Terrain();

	Terrain() : Entity(0, Map::GRID_SIZE, Map::GRID_SIZE, 0.0f, 1.0f, -199.9f, 0.0f) {
		physicsNode.setBoundingCircle(BOUNDING_CIRCLE);
		collidableNode.boundingRadius = BOUNDING_CIRCLE;
		collidableNode.isFixed = true;
	}

	Terrain(const Vector2 pos) : Terrain() {
		this->physicsNode.setPosition(pos);
	}

	Terrain(const int gridX, const int gridY) : Terrain() {
		this->setGridPosition(gridX, gridY);
	};

private:

};

