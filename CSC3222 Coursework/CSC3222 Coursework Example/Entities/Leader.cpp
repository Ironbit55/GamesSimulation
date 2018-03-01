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
	
	//alright this is a bit dodgy but we don't want the leader
	//to execute the follower update logic so
	//just call entity

	Entity::update(msec);

	//remind me to rethink the whole entity updating deal
}
