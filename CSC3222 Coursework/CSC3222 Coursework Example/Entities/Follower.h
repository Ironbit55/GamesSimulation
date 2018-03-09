#pragma once
#include "../Entity.h"


class Follower :
	public Entity
{
public:
	Follower(const float rotation = 0.0f) : Entity(0.0f, 10.0f, 10.0f, 1000.0f, 1.0f, -199.9f, rotation){
		//physicsNode.setBoundingCircle(4.0f);
		collidableNode.boundingRadius = 15.0f;
		collidableNode.isFixed = false;
	};
	Follower(const Vector2 pos, const float rotation = 0.0f) : Follower(rotation){
		physicsNode.setPosition(pos);
	};
	Follower(const int gridX, const int gridY, const float rotation = 0.0f) : Follower(rotation){
		this->setGridPosition(gridX, gridY);
	};
	virtual void update(float msec);

	Vector2 leaderLocation;
	bool followLeader;

	void setTerrainType(TileType terrain) {
		terrainType = terrain;
	}

protected:
	TileType terrainType;

	void limitVelocityByTerrain();
};

