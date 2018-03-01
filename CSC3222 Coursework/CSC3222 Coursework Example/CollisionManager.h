#pragma once

#include "Components/PhysicsNode.h"
#include "../nclgl/Vector2.h";

/**
 * contains functions to handle collisions between Shapes of types Square and Circle
 */
class CollisionManager{
public:
	CollisionManager();
	~CollisionManager();
	

	bool static circleCircleCollision(PhysicsNode& c1, PhysicsNode& c2);

	//bool static squareSquareCollision(Square* s1, Square* s2);

	//bool static circleSquareCollision(Circle* c1, Square* s1);

};

