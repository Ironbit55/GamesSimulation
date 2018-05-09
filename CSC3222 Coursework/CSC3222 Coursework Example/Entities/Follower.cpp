#include "Follower.h"

void Follower::update(float msec){

	//this is the same logic as the dragon follow...
	//I guess we could refactor it into some generic follow behaviour
	//component

	//tell entity to look at target
	//then move foward, (therefore toward target)
	//lookAt(leaderLocation);

	Vector2 toLeaderDir = leaderLocation - physicsNode.getPosition();
	toLeaderDir.Normalise();
	if (followLeader) {
		//velocityNode.applyVelocity(toLeaderDir, 0.04f);
		velocityNode.applyForce(toLeaderDir * 100.0f);
	} else {
		velocityNode.setVelocity(Vector2(0.0f, 0.0f));
	}


	Entity::update(msec);
}

void Follower::limitVelocityByTerrain(){
	float maxVelocity = 5;
	float targetVelocity = getTerrainVelocity();
	float baseVelocity = feetToPixels(5);
	
	velocityNode.setDampingFactor(targetVelocity / baseVelocity);
	velocityNode.setMaxSpeed(baseVelocity);
}

float Follower::getTerrainVelocity() {
	switch (terrainType) {
	case RUBBLE:
		return feetToPixels(2);
	default: //includes floor
		return feetToPixels(5);
	}
}
