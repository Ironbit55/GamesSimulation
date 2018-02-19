#pragma once
#include "Physics.h"


Physics::Physics()
{
	numRaiders = 16;

	for (int i = 0; i < numRaiders - 1; i++)
	{
		Vector2 tempPosition = Vector2(0 + 27 * i, -80.0f);
		float tempRotation = i*20.0f;
		//Follower tempFollower = Follower(tempPosition, tempRotation);
		Follower tempFollower = Follower(24 + i, 15, tempRotation);
		raiders.push_back(tempFollower);
	}
	 //leader = &raiders.back();
	leader = Leader(30, 17, 20.0f);
	raiders.push_back(leader);
	leader.physicsNode.setGridPosition(40, 17);
	Vector2 mapPosition = Vector2(0.0f, 100.0f);
	Vector3 mapScale = Vector3(864.0f, 540.0f, 100.0f);
	map = PhysicsNode(mapPosition, -200.0f, 0.0f, mapScale);

	
	Vector2 dragonPos = Vector2(-300.0f, 90.0f);

	dragon = Dragon(dragonPos);

	/*dragon.position = Vector3(-300.0f, 90.0f, -199.5f);
	dragon.rotation = 0.0f;
	dragon.scale = Vector3(50.0f, 50.0f, 1.0f);*/

	dragonState = 1;

	Vector2 breathPosition = Vector2(0.0f, -50.0f);
	Vector3 breathScale = Vector3(2.0f, 1.0f, 1.0f);
	//depth is relative to parent... which probably isn't what we want
	breath = PhysicsNode(breathPosition, -0.3f, 270.0f, breathScale);

	breathState = 1;
}

Physics::~Physics()
{
	raiders.clear();
}

void Physics::UpdatePhysics(float msec)
{

	/* Placeholder functionality to show things moving on screen
	Note that our physics update is parcelled away from our renderer. So
	long as our SceneNode has a pointer to its associated PhysicsData it
	should update reliably. Note that as these pointers are set at the
	beginning of execution, we should take care when messing around with
	the contents of our raiders <vector> */
	
	float shift;

	shift = 0.3*msec;

	/* Here, we simply rotate each raider counter-clockwise by a value
	relative to framerate. */

	for (int i = 0; i < numRaiders; i++)
	{
		raiders.at(i).physicsNode.updateRotation(shift);
	}

	/* This segment demonstrates a simple finite state machine. State 1 is
	analogous to 'move right'. Once the dragon has moved right beyond a
	specific threshold (in this example, 100 units from origin), it flips
	to State 0, 'move left', and continues until it is -320 units from
	origin. Repeat ad nauseum. You should compartmentalise this decision
	making away from your physics system - it is included here for
	illustrative purposes only.	*/

	shift = 0.1*msec;

	if (dragon.physicsNode.getPosition().x > 100.0f)
	{
		dragonState = 0;
	}
	else if(dragon.physicsNode.getPosition().x < -320.0f)
	{
		dragonState = 1;
	}

	if (dragonState == 1)
	{
		dragon.physicsNode.moveX(shift);
	}
	else
	{
		dragon.physicsNode.moveX(-shift);
	}

	/* Note also, as highlighted in the comments in Renderer.cpp, that
	we do not explicitly change the position of the fiery breath - but
	the breath moves in any case. This is because it inherits its base
	coordinates from its parent (the dragon) in our scene structure. It
	also inherits its orientation from the dragon - to see this in
	action, uncomment the line of code below: */

	dragon.physicsNode.updateRotation(-shift);

	/* Lastly, there may be times when we want to control the scale of
	an object. If that object collides, it makes sense for its physics
	data to be aware of, or govern, this change in scale. Our SceneNode
	is adapted to read in an object's scale each frame from its associated
	PhysicsData just in case it has changed - here is an example of that
	applied to the dragon's breath weapon. */

	if (breath.getScale().x > 100.0f)
	{
		breathState = 0;
		breath.setScaleXY(100, 50);
	}
	else if (breath.getScale().x < 2.0f)
	{
		breathState = 1;
		breath.setScaleXY(2.0f, 1.0f);
	}

	if (breathState == 1)
	{
		breath.addToScaleXY(1.5*shift, 0.75*shift);
	}
	else
	{
		breath.addToScaleXY(-2 * shift, -shift);
	}

}