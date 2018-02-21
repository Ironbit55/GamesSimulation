#include "Leader.h"

void Leader::update(float msec){
	
	if(leaderControler.moveForward){
		velocityNode.setVelocity(physicsNode.getForwardDirection(), 0.10f);
	}else{
		//a little hacky but...
		velocityNode.setVelocity(Vector2(0.0f, 0.0f));
	}

	if(leaderControler.rotateRight){
		physicsNode.updateRotation(-0.20f * msec);
	}
	if (leaderControler.rotateLeft) {
		physicsNode.updateRotation(0.20f * msec);
	}
	
	Follower::update(msec);
}
