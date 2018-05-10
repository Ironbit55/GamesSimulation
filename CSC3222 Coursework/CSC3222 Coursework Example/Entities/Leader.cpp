#include "Leader.h"

void Leader::update(float msec){
	
	if(leaderControler.moveForward){
		//velocityNode.setVelocity(physicsNode.getForwardDirection(), 27.0f);
		//velocityNode.applyVelocity(physicsNode.getForwardDirection(), feetToPixels(5));
		//float deltaVelocity = getTerrainVelocity();
		//if (velocityNode.getVelocity().Length() > 0.0f) {
		//	deltaVelocity = getTerrainVelocity() / velocityNode.getVelocity().Length();
		//}
		velocityNode.applyVelocity(physicsNode.getForwardDirection(), feetToPixels(BASE_SPEED_FEET));
	}else{
		//a little hacky but...
		//float deltaVelocity = 0 / velocityNode.getVelocity().Length();
		//velocityNode.applyVelocity(physicsNode.getForwardDirection(), deltaVelocity);
		velocityNode.setVelocity(Vector2(0.0f, 0.0f));
		
	}

	if(leaderControler.rotateRight){
		physicsNode.updateRotation(-0.20f * msec);
	}
	if (leaderControler.rotateLeft) {
		physicsNode.updateRotation(0.20f * msec);
	}
	
	limitVelocityByTerrain();
	//velocityNode.setMaxSpeed(feetToPixels(7));
	//alright this is a bit dodgy but we don't want the leader
	//to execute the follower update logic so
	//just call entity

	Entity::update(msec);
	leaderControler.colliding = false;

	//remind me to rethink the whole entity updating deal
}
