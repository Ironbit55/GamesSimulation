#include "VelocityNode.h"


void VelocityNode::update(float msec){
	float deltaSec = msec / 1000.0f;

	//acceleration has already been calculated as we added forces
	
	//acceleration = force  * inverseMass;
	//put some limit on acceleration

	
	//calculate velocity of next frame
	velocity = velocity + (acceleration * deltaSec);
	velocity = velocity * (drag);
	//limit velocity accordng to max speed

	float currentSpeed = velocity.Length();
	if (currentSpeed > maxSpeed){
		velocity.Normalise();
		velocity *= maxSpeed;
	}else if(currentSpeed < MIN_VELOCITY){
		velocity = Vector3();
	}


	//uses new velocity to inform new displacement
	physicsNode->move(Vector2(velocity.x, velocity.y) * deltaSec);

	force.ToZero();
	acceleration.ToZero();
}
