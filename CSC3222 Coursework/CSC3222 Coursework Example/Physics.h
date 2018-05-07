#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"
#include "../CSC3222 Coursework Example/Entity.h"
#include "../CSC3222 Coursework Example/Entities/Dragon.h"
#include "../CSC3222 Coursework Example/Entities/Leader.h"
#include "../CSC3222 Coursework Example/Entities/Follower.h"
#include "Components/PhysicsNode.h"


using std::vector;


/* Our placeholder physics system includes an update function, and physics data for each entity
in our environment. We also include a single integer to inform the dragon's behaviour and breath
weapon scaling.*/

class Physics {
public:
	Physics();
	~Physics();

	void UpdatePhysics(float msec);
	bool loadTileMap();
	TileType getTileAt(int x, int y) {
		if (x > 0 && y > 0 && x < Map::TILES_X && y < Map::TILES_Y) {
			int flippedY = Map::TILES_Y - y;
			return tileMap[flippedY * Map::TILES_X + x];
		}
		return INVALID_TILE;

		
	}

	PhysicsNode map;
	Dragon dragon;
	PhysicsNode breath;
	vector<Follower> raiders;
	Leader leader;
	TileType* tileMap;

	
private:
	int numRaiders;
	int	dragonState;
	int breathState;
};