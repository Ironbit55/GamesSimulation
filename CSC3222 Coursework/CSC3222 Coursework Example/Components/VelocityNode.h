/*
 * Author: Ed Curran (b5021040)
 * Class: VelocityNode
 * Description: used to alter the position of an entity using velocity.
 * Velocity can be altered directly and by and applying forces
 *
 * I decided to use the unity model of forces cus i dunno
 * it had good documentation so I can copy it.
 * This means if you want to constantly apply a force you have to do
 * it yourself every update
 * 
 * alternative implementation I considered is have concept of force. which is constantly applied on update untill removed
 * and impulse which is applied in update only once then removed. 
 * applyForce would result in constant acceleration
 * applyImpulse would result in constant velocity
 */


#pragma once
#include "PhysicsNode.h"

class VelocityNode{
public:
	VelocityNode(PhysicsNode* physicsNode, float maxSpeed, float mass = 1.0f, float d = 0.95f) : physicsNode(physicsNode), mass(mass), maxSpeed(maxSpeed), inverseMass(1.0f / mass), drag(d) {};
	//~VelocityNode();

	void update(float msec);





	/*
	 * timestep dependent
	 * mass dependent
	 * changes acceleration dependent on mass
	 * force will be divided by mass and applied to acceleration
	 * velocity is dependent on acceleration and timestep therefore
	 * changes velocity dependent on mass and timestep. 
	 */
	void applyForce(Vector2 applyForce){
		acceleration = applyForce * inverseMass;

		//I'm keeping force around cus i have a sneaking suspicion it might be usefull
		//but...
		force += applyForce;
	}

	/*
	* timestep dependent
	* mass independent
	* changes acceleration independent of mass
	* velocity is dependent on acceleration and timestep therefore
	* changes velocity dependent on timestep.
	*/
	void applyAcceleration(Vector2 accelerationChange){
		//yeah its kinda wierd multiplying by mass here
		//we should really change velocity update to use acceleration directly
		//and apply force modifies acceleration
		acceleration += accelerationChange;

		//I'm keeping force around cus i have a sneaking suspicion it might be usefull
		//but...
		force += accelerationChange * mass;
		
	}



	/* 
	 * timestep independent
	 * mass dependent
	 * Changes velocity dependent on mass.
	 * Impulse will be divided by mass and applied to velocity
	 * Apply the impulse force instantly with a single function call.
	 */
	void applyImpulse(const Vector2 impulse) {
		applyVelocity(impulse * inverseMass);
	}

	/*
	 * timestep independent
	 * mass independent
	 * Changes velocity independent of mass
	 * Apply the velocity change instantly with a single function call. 
	*/
	void applyVelocity(const Vector2 applyVelocity) {
		velocity += Vector3(applyVelocity);
	}

	/*
	 * takes into account the entities mass to apply the given imupulse to velocity
	 * I dunno if this is normal but makes sense to me... kinda
	 */
	void applyImpulse(const Vector2 dir, float magnitude) {
		magnitude *= inverseMass;
		//dir.Normalise();
		applyVelocity(dir * magnitude);
	}

	

	void applyVelocity(const Vector2 dir, const float amount) {
		
		velocity += Vector3(dir * amount);
	}

	void setVelocity(const Vector2 v){
		velocity = v;
	}

	void setVelocity(const Vector2 dir, const float amount) {
		Vector3 test = dir * amount;
		velocity = dir * amount;
	}
	void setMass(const float mass){
		this->mass = mass;
		inverseMass = 1.0f / mass;
	}

	float getMass() const { return mass; }

	float getInverseMass() const { return inverseMass; }

	Vector2 getVelocity () const {
		return Vector2(velocity.x, velocity.y);
	}

	void setMaxSpeed(const float maxSpeed){
		this->maxSpeed = maxSpeed;
	}

	void setPhysicsNode(PhysicsNode* p){
		physicsNode = p;
	}


private:
	PhysicsNode* physicsNode;
	float mass;
	float inverseMass;
	float maxSpeed;
	float drag;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 force;
	const float MIN_VELOCITY = 0.001f;
};

