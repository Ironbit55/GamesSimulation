#pragma once
#include "PhysicsNode.h"

class VelocityNode{
public:
	VelocityNode(PhysicsNode* physicsNode, float maxSpeed, float mass = 1.0f) : physicsNode(physicsNode), mass(mass), maxSpeed(maxSpeed), inverseMass(1/mass) {};
	//~VelocityNode();

	void update(float msec);
	void applyForce(Vector2 applyForce);

	void applyImpulse(const Vector2 f) {
		applyVelocity(f);
	}

	void applyImpulse(const Vector2 dir, const float magnitude) {
		
		//dir.Normalise();
		applyVelocity(dir * magnitude);
	}


	void applyVelocity(const Vector2 applyVelocity){
		velocity += Vector3(applyVelocity);
	}
	void applyVelocity(const Vector2 dir, const float amount) {
		
		velocity += Vector3(dir * amount);
	}

	void setVelocity(const Vector2 v){
		velocity = v;
	}

	void setVelocity(const Vector2 dir, const float amount) {
		velocity = dir * amount;
	}
	void setMass(const float mass){
		this->mass = mass;
		inverseMass = 1 / mass;
	}

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
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 force;
};

