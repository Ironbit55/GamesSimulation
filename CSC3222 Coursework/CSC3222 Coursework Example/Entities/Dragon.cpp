#include "Dragon.h"

void Dragon::update(float msec){

	//tell entity to look at target
	//then move foward, (therefore toward target)
	lookAt(targetLocation);
	if (moveToTarget) {
		velocityNode.setVelocity(physicsNode.getForwardDirection(), 0.04f);
	}else{
		velocityNode.setVelocity(Vector2(0.0f, 0.0f));
	}

	//could also set velocity in direction of target
	//and rotate according to dir
	Entity::update(msec);
}
