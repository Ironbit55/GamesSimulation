#pragma once

#include "../CSC3222 Coursework Example/Entity.h";
#include "../nclgl/Vector2.h";

/**
 * contains functions to handle collisions between Shapes of types Square and Circle
 */
class CollisionManager{
	CollisionManager();
	~CollisionManager();
	
public: 
	bool static lineCircleCollision(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePos, float circleRadius, CollisionData& collisionDataOut);
	bool static circleCircleCollision(Vector2 pos1, float radius1, Vector2 pos2, float radius2, CollisionData& collisionDataOut);

	bool static circleCircleCollision(PhysicsNode& c1, PhysicsNode& c2);

	bool static entityCircleInterfaceDetection(Entity& e1, Entity& e2, CollisionData& collisionDataOut);

	//returns impluse j
	float static calculateCollisionImpulse(Entity& e1, Entity& e2, CollisionData data);

	void static resolveCollision(Entity& e1, Entity& e2, CollisionData data);

	//bool static squareSquareCollision(Square* s1, Square* s2);

	//bool static circleSquareCollision(Circle* c1, Square* s1);

};

