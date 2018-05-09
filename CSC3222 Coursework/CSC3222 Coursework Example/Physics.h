#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"
#include "../CSC3222 Coursework Example/Entity.h"
#include "../CSC3222 Coursework Example/Entities/Dragon.h"
#include "../CSC3222 Coursework Example/Entities/Leader.h"
#include "../CSC3222 Coursework Example/Entities/Follower.h"
#include "Components/PhysicsNode.h"
#include "Terrain.h"


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
		if (x >= 0 && y >= 0 && x < Map::TILES_X && y < Map::TILES_Y) {
			int flippedY = Map::TILES_Y - y;
			return tileMap[flippedY * Map::TILES_X + x];
		}
		return INVALID_TILE;

		
	}

	Vector2 getTileOrigin(int x, int y) {
		
	}

	PhysicsNode map;
	Dragon dragon;
	PhysicsNode breath;
	vector<Follower> raiders;
	vector<Entity> boulderColliders;
	vector<Terrain> wallColliders;
	Leader leader;
	TileType* tileMap;

protected:
	void fillTerrainColliders() {
		
	}

	void addTileCollider(int x, int y, TileType tile){
		//addBoulderCollider(20, 3, Map::GRID_SIZE * 3);
		//addBoulderCollider(9, 12, Map::GRID_SIZE * 1.5f);
		//addBoulderCollider(34, 17, Map::GRID_SIZE * 1.5f);
		//addBoulderCollider(11, 22, Map::GRID_SIZE * 2);
		//addBoulderCollider(33, 28, Map::GRID_SIZE * 2);
		//addBoulderCollider(47, 29, Map::GRID_SIZE * 1.0f);
		//addBoulderCollider(33, 30, Map::GRID_SIZE * 2.0f);
		//addBoulderCollider(47, 30, Map::GRID_SIZE * 2);
		//addBoulderCollider(30, 10, Map::GRID_SIZE * 0.5);
		if(tile == TileType::WALL){
			if ((getTileAt(x - 1, y) == (TileType::FLOOR)) || (getTileAt(x + 1, y) == (TileType::FLOOR)) ||
				(getTileAt(x, y - 1) == (TileType::FLOOR)) || (getTileAt(x, y + 1) == (TileType::FLOOR))) {
				boulderColliders.push_back(Terrain(x, y));

				Follower tempFollower = Follower(x, y, 0.0f);


				raiders.push_back(tempFollower);
				raiders.at(raiders.size() - 1).followLeader = false;
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
		boulderColliders.push_back(Terrain(x, y));
		boulderColliders.at(boulderColliders.size() - 1).collidableNode.boundingRadius = boundingRadius;
		boulderColliders.at(boulderColliders.size() - 1).physicsNode.move(offset);

		Follower tempFollower = Follower(x, y, 0.0f);


		raiders.push_back(tempFollower);
		raiders.at(raiders.size() - 1).followLeader = false;
		raiders.at(raiders.size() - 1).setColour(Vector4(1.0f, 0, 0.0f, 1.0f));
		raiders.at(raiders.size() - 1).physicsNode.move(offset);
	}

	
private:
	int numRaiders;
	int	dragonState;
	int breathState;
};