#include "Dragon.h"

void Dragon::update(float msec){

	//tell entity to look at target
	//then move foward, (therefore toward target)
	lookAt(targetLocation);
	
	//physicsNode.setRotation(Vector2::Normalise(velocityNode.getVelocity()));
	if (moveToTarget) {
		movementManager.applySeek(targetLocation, feetToPixels(20));
		//velocityNode.applyVelocity(physicsNode.getForwardDirection(), feetToPixels(DRAGON_SPEED_FEET));
	}else{
		velocityNode.setVelocity(Vector2(0.0f, 0.0f));
	}

	movementManager.applyObstacleAvoidance();
	//velocityNode.setMaxSpeed(feetToPixels(DRAGON_SPEED_FEET));
	velocityNode.setMaxSpeed(feetToPixels(DRAGON_SPEED_FEET));
	//velocityNode.setDampingFactor(feetToPixels(DRAGON_SPEED_FEET) / velocityNode.getVelocity().Length());
	movementManager.update();
	//could also set velocity in direction of target
	//and rotate according to dir


	Entity::update(msec);
}

void Dragon::setObstacles(std::vector<Entity>* obstacles) {
	movementManager.addObstacles(obstacles);
}
