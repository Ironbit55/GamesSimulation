#pragma once
#include "Physics.h"
#include "../../nclgl/Window.h"
#include "CollisionManager.h"


Physics::Physics()
{
	numRaiders = 15;

	for (int i = 0; i < numRaiders - 1; i++){
		float tempRotation = i*20.0f;
		Follower tempFollower = Follower(13 + i * 2, 20 - i, tempRotation);

		tempFollower.velocityNode.applyForce(Vector2(0, 1) * 10.0f);
		raiders.push_back(tempFollower);
		raiders.at(i).physicsNode.setRotation(Vector2(-1, 0));
		raiders.at(i).followLeader = true;
	}
	
	 //push leader to followers/raider list

	//add leader to raiders
	leader = Leader(30, 20, 20.0f);
	leader.physicsNode.setRotation(Vector2(0, 1));
	leader.leaderControler.moveForward = true;
	
	//leaderPtr->velocityNode.applyVelocity(Vector2(0.0f, -0.02f));

	Vector2 mapPosition = Vector2(0.0f, 0.0f);
	Vector3 mapScale = Vector3(Map::MAP_IMAGE_HALF_WIDTH, Map::MAP_IMAGE_HALF_HEIGHT, 100.0f);
	map = PhysicsNode(mapPosition, mapScale, -200.0f);

	
	Vector2 dragonPos = Vector2(-300.0f, 90.0f);
	dragon = Dragon(dragonPos);

	Vector2 dir = Vector2(0, 1);
	dir.Normalise();
	dragon.physicsNode.setRotation(dir);
	dragonState = 1;

	Vector2 breathPosition = Vector2(0.0f, -50.0f);
	Vector3 breathScale = Vector3(2.0f, 1.0f, 1.0f);
	//depth is relative to parent... which probably isn't what we want
	breath = PhysicsNode(breathPosition, breathScale, -0.3f, 270.0f);

	breathState = 1;
}

Physics::~Physics()
{
	raiders.clear();
}

void Physics::UpdatePhysics(float msec)
{
	//handle input
	leader.leaderControler.moveForward = Window::GetKeyboard()->KeyDown(KEYBOARD_UP);
	leader.leaderControler.rotateLeft = Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT);
	leader.leaderControler.rotateRight = Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT);

	if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)){
		dragon.moveToTarget = !dragon.moveToTarget;
	}

	dragon.targetLocation = leader.physicsNode.getPosition();

	CollisionData collisionData;
	for (int i = 0; i < numRaiders - 1; i++){
		//point follower to look at dragon
		//this should probably be in follower update logic
		Follower& followerA = raiders.at(i);
		followerA.lookAt(dragon.physicsNode.getPosition());
		followerA.leaderLocation = leader.physicsNode.getPosition();

		followerA.update(msec);
		followerA.followLeader = true;
		
		//check raider raider collision
		
		for (int k = 0; k < numRaiders - 1; k++) {
			Follower& followerB = raiders.at(k);
			if(i != k && CollisionManager::entityCircleInterfaceDetection(followerA, followerB, collisionData)){
				//followers are colliding
				//followerA.followLeader = false;
				followerA.physicsNode.colour = Vector4(1.0f, 0.5f, 0.2f, 0.999f);
				//followerB.followLeader = false;
				followerB.physicsNode.colour = Vector4(1.0f, 0.5f, 0.2f, 0.999f);
				CollisionManager::resolveCollision(followerA, followerB, collisionData);
			}
		}
	}
	
	leader.update(msec);
	dragon.update(msec);

	//ideally loop through all entities
	//if collidable then detect collision and respond
	//if two collidables are children (or hackily: both fixed) then don't collide

	/* Placeholder functionality to show things moving on screen
	Note that our physics update is parcelled away from our renderer. So
	long as our SceneNode has a pointer to its associated PhysicsData it
	should update reliably. Note that as these pointers are set at the
	beginning of execution, we should take care when messing around with
	the contents of our raiders <vector> */
	
	float shift;

	shift = 0.3*msec;



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
		//dragon.physicsNode.moveX(shift);
	}
	else
	{
		//dragon.physicsNode.moveX(-shift);
	}

	/* Note also, as highlighted in the comments in Renderer.cpp, that
	we do not explicitly change the position of the fiery breath - but
	the breath moves in any case. This is because it inherits its base
	coordinates from its parent (the dragon) in our scene structure. It
	also inherits its orientation from the dragon - to see this in
	action, uncomment the line of code below: */

	//dragon.physicsNode.updateRotation(-shift);

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