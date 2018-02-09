#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"
#include "../CSC3222 Coursework Example/Entity.h"

using std::vector;



/* Our placeholder physics system includes an update function, and physics data for each entity
in our environment. We also include a single integer to inform the dragon's behaviour and breath
weapon scaling.*/

class Physics {
public:
	Physics();
	~Physics();

	void UpdatePhysics(float msec);

	PhysicsData map;
	Entity dragon;
	PhysicsNode breath;
	vector<Entity> raiders;
	
private:
	int numRaiders;
	int	dragonState;
	int breathState;
};