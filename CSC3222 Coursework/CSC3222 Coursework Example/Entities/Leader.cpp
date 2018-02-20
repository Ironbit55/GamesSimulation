#include "Leader.h"

void Leader::update(float msec){
	
	if(leaderControler.moveForward){
		//foward is up in y
		Vector3 forward = Vector3(0, 1, 0);

		Quaternion::RotatePointByQuaternion(physicsNode.getQrotation(), forward);
		//foward should now point in direction entity is facing
		
		//apply velocity in this direction
		velocityNode.setVelocity(Vector2(forward.x, forward.y), 0.06f);
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
