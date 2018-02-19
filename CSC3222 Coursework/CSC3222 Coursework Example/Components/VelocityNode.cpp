#include "VelocityNode.h"


void VelocityNode::update(float msec){
	acceleration = force * inverseMass;
	//put some limit on acceleration

	
	//calculate velocity of next frame
	velocity = velocity + (acceleration * msec);
	//limit velocity accordng to max speed
	if (velocity.Length() > maxSpeed){
		velocity.Normalise();
		velocity *= maxSpeed;
	}

	//uses new velocity to inform new displacement
	physicsNode->move(Vector2(velocity.x, velocity.y) * msec);

	acceleration.ToZero();
}
