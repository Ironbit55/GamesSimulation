#include "Follower.h"

void Follower::update(float msec){
	std::cout << "FOLLOWER UPDATE";

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
