#pragma once
#include "../Entity.h"
#include <vector>
#include "../MovementManager.h"


class Follower :
	public Entity
{
public:
	Follower(const float rotation = 0.0f) : Entity(0.0f, 10.0f, 10.0f, 1000.0f, 1.0f, -199.9f, rotation), movementManager(velocityNode, collidableNode, feetToPixels(10)){
		//physicsNode.setBoundingCircle(4.0f);
		collidableNode.boundingRadius = 9.0f;
		collidableNode.isFixed = false;
	};
	Follower(const Vector2 pos, const float rotation = 0.0f) : Follower(rotation){
		physicsNode.setPosition(pos);
	};
	Follower(const int gridX, const int gridY, const float rotation = 0.0f) : Follower(rotation){
		this->setGridPosition(gridX, gridY);
	};

	Follower(const Follower& src) : Entity(src),
		movementManager(velocityNode, collidableNode, src.movementManager.getMaxForce(), src.movementManager.MAX_LOOKAHEAD),
		leader(src.leader), followLeader(src.followLeader) {
		
	};

	Follower operator=(const Follower& rhs) {
		return Follower(rhs);
	}

	virtual void update(float msec);

	Vector2 leaderLocation;
	VelocityNode* dragon;
	bool followLeader;

	void setFollowers(std::vector<Follower>& followers) {
		//this->followers = followers;
		std::vector<Entity*> flock;
		for (auto it = followers.begin(); it != followers.end(); ++it) {
			flock.push_back(&(*it));
		}
		movementManager.setFlock(flock);
	}

	void setLeader(Entity* leader){
		this->leader = leader;
	}

	void setObstacles(std::vector<Entity>* obstacles){
		movementManager.addObstacles(obstacles);
	}

	void setPath(std::vector<Node>& path){
		followPath = true;
		movementManager.setPath(path);
	}

	//void setTerrainType(TileType terrain) {
	//	terrainType = terrain;
	//}

protected:
	//TileType terrainType;

	void limitVelocityByTerrain();

	float getTerrainVelocity();

	//std::vector<Follower>* followers;

	MovementManager movementManager;

	Entity* leader;
	bool followPath = false;
};



