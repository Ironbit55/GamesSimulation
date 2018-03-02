#pragma once

#include <vector>
#include "../../nclgl/Vector3.h"
#include "../CSC3222 Coursework Example/Entity.h"
#include "../CSC3222 Coursework Example/Entities/Dragon.h"
#include "../CSC3222 Coursework Example/Entities/Leader.h"
#include "../CSC3222 Coursework Example/Entities/Follower.h"
#include "Components/PhysicsNode.h"


using std::vector;


/* Our placeholder physics system includes an update function, and physics data for each entity
in our environment. We also include a single integer to inform the dragon's behaviour and breath
weapon scaling.*/

class Physics {
public:
	Physics();
	~Physics();

	void UpdatePhysics(float msec);

	PhysicsNode map;
	Dragon dragon;
	PhysicsNode breath;
	vector<Follower> raiders;
	Leader leader;
	
private:
	int numRaiders;
	int	dragonState;
	int breathState;
};