#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"
#include "../CSC3222 Coursework Example/Entity.h"
#include "../CSC3222 Coursework Example/Entities/Dragon.h"
#include "../CSC3222 Coursework Example/Entities/Leader.h"
#include "../CSC3222 Coursework Example/Entities/Follower.h"
#include "Components/PhysicsNode.h"
#include "Terrain.h"
#include "TileMap.h"
#include "Pathfinding.h"


class Renderer;
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
	//TileType getTileAt(int x, int y) {
	//	if (x >= 0 && y >= 0 && x < Map::TILES_X && y < Map::TILES_Y) {
	//		int flippedY = Map::TILES_Y - y;
	//		return tileMap[flippedY * Map::TILES_X + x];
	//	}
	//	return INVALID_TILE;

	//	
	//}

	Vector2 getTileOrigin(int x, int y) {
		
	}

	void initialse(Renderer* renderer);

	void pathRaiderToPool();
	void pathRaiderToHoard();

	PhysicsNode map;
	Dragon dragon;
	PhysicsNode breath;
	Renderer* renderer;
	vector<Follower> raiders;
	vector<Entity> terrainColliders;
	vector<Terrain> wallColliders;
	Leader leader;
	//TileType* tileMap;
	TileMap tileMap;
	Pathfinding pathfinding;

protected:
	
	void addTileCollider(int x, int y, TileType tile){

		if(tile == TileType::WALL){
			if ((tileMap.getTileAt(x - 1, y) == (TileType::FLOOR)) || (tileMap.getTileAt(x + 1, y) == (TileType::FLOOR)) ||
				(tileMap.getTileAt(x, y - 1) == (TileType::FLOOR)) || (tileMap.getTileAt(x, y + 1) == (TileType::FLOOR))) {
				terrainColliders.push_back(Terrain(x, y));
				terrainColliders.at(terrainColliders.size() - 1).setColour(Vector4(0, 1, 1, 0.99f));
			}
		}else if(tile == TileType::BOULDER){
			//std::cout << "boulder at: (" << x + ", " << y << ")";
			std::cout << "boulder at: ";
			std::cout << x;
			std::cout << ", " << y << "\n";
			//addBoulderCollider(x, y);
		}


	}

	void addBoulderCollider(int x, int y, float boundingRadius, Vector2 offset = Vector2()) {
		terrainColliders.push_back(Terrain(x, y));
		terrainColliders.at(terrainColliders.size() - 1).collidableNode.boundingRadius = boundingRadius;
		terrainColliders.at(terrainColliders.size() - 1).physicsNode.move(offset);
		terrainColliders.at(terrainColliders.size() - 1).physicsNode.setScaleXY(boundingRadius, boundingRadius);
		terrainColliders.at(terrainColliders.size() - 1).setColour(Vector4(1, 0, 0, 0.99f));
	}

	int randInt(int min, int max) {
		return rand() % max + min;
	}

	
private:
	int numRaiders;
	int	dragonState;
	int breathState;
};