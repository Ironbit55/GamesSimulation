#pragma once
#include "Physics.h"
#include "../../nclgl/Window.h"
#include "CollisionManager.h"

bool Physics::loadTileMap() {
	
	//add terrain colliders
	for (int y = 0; y < Map::TILES_Y; y++) {
		for (int x = 0; x < Map::TILES_X; x++) {
			//add collider based on tile
			TileType tileType = tileMap.getTileAt(x, y);
			addTileCollider(x, y, tileType);
		}
	}

	addBoulderCollider(19, 4, Map::GRID_SIZE * 1);
	addBoulderCollider(23, 5, Map::GRID_SIZE * .7f, Vector2(Map::GRID_SIZE * 0.5f, Map::GRID_SIZE * 0.5f));
	addBoulderCollider(31, 9, Map::GRID_SIZE * 1.0f, Vector2(Map::GRID_SIZE * 0.2f, -Map::GRID_SIZE * 0.5f));
	addBoulderCollider(8, 10, Map::GRID_SIZE * .8f, Vector2(0, 0));
	addBoulderCollider(8, 13, Map::GRID_SIZE * 2.0f);
	addBoulderCollider(35, 18, Map::GRID_SIZE * 1.0f, Vector2(0, -Map::GRID_SIZE * 0.5f));
	addBoulderCollider(10, 23, Map::GRID_SIZE * 1.5f, Vector2(-Map::GRID_SIZE * 0.5f, 0));
	addBoulderCollider(27, 25, Map::GRID_SIZE * 1.5f, Vector2(-Map::GRID_SIZE * 0.5f, Map::GRID_SIZE * 0.1f));
	addBoulderCollider(34, 29, Map::GRID_SIZE * 1.5f, Vector2(-Map::GRID_SIZE * 0.5f, 0));
	addBoulderCollider(48, 29, Map::GRID_SIZE * 1.0f);

	pathfinding.fillGraphFromGrid();
	std::vector<Node> path;
	pathfinding.aStarSearch(pathfinding.getGridNodeId(40, 22), pathfinding.getGridNodeId(34, 6), path);

	for (int i = 0; i < path.size(); ++i){
		addBoulderCollider(path.at(i).id % Map::TILES_X, path.at(i).id / Map::TILES_X, Map::GRID_SIZE *0.5);
		//addBoulderCollider(Map::TILES_X, Map::TILES_Y, Map::GRID_SIZE *0.5, path.at(i).position);
	}
	
	return true;
}
Physics::Physics() : pathfinding(tileMap)
{
	loadTileMap();
	numRaiders = 15;

	//leader = Leader(0, 0, 20.0f);
	leader.setGridPosition(30, 15);
	leader.physicsNode.setRotation(Vector2(0, 1));

	for (int i = 0; i < numRaiders - 1; i++){
		float tempRotation = i*20.0f;
		Follower tempFollower(13 + (i * 3) % 8, 20 - (i / 8) * 2, tempRotation); 

		tempFollower.velocityNode.applyForce(Vector2(0, 1) * 10.0f);
		raiders.push_back(tempFollower);
		raiders.at(i).physicsNode.setRotation(Vector2(-1, 0));
		raiders.at(i).followLeader = true;
		raiders.at(i).setLeader(&leader);
	}

	for (int i = 0; i < raiders.size(); i++) {
		raiders.at(i).setFollowers(raiders);
		raiders.at(i).setObstacles(&terrainColliders);
	}
	
	 //push leader to followers/raider list

	//add leader to raiders


	int testX = Map::worldToGridPositionX(leader.physicsNode.getPositionX());
	int testY = Map::worldToGridPositionY(leader.physicsNode.getPositionY());
	
	//leaderPtr->velocityNode.applyVelocity(Vector2(0.0f, -0.02f));

	Vector2 mapPosition = Vector2(0.0f, 0.0f);
	Vector3 mapScale = Vector3(Map::MAP_IMAGE_HALF_WIDTH, Map::MAP_IMAGE_HALF_HEIGHT, 100.0f);
	map = PhysicsNode(mapPosition, mapScale, -200.0f);

	
	Vector2 dragonPos = Vector2(-200.0f, 0.0f);
	dragon = Dragon(dragonPos);

	Vector2 dir = Vector2(0, 1);
	dir.Normalise();
	dragon.physicsNode.setRotation(dir);
	dragonState = 1;

	Vector2 breathPosition = Vector2(0.0f, -50.0f);
	Vector3 breathScale = Vector3(2.0f, 1.0f, 1.0f);
	//depth is relative to parent... which probably isn't what we want
	breath = PhysicsNode(breathPosition, breathScale, -0.3f, 270.0f);

	addTileCollider(30, 10, TileType::BOULDER);

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
	for (int i = 0; i < raiders.size(); i++){
		//point follower to look at dragon
		//this should probably be in follower update logic
		Follower& followerA = raiders.at(i);
		followerA.lookAt(dragon.physicsNode.getPosition());
		followerA.leaderLocation = leader.physicsNode.getPosition();

		followerA.update(msec);
		followerA.followLeader = true;
		
		//check raider raider collision
		
		for (int k = 0; k < raiders.size(); k++) {
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

		for (auto it = terrainColliders.begin(); it != terrainColliders.end(); ++it) {
			if (CollisionManager::entityCircleInterfaceDetection(followerA, *it, collisionData)) {
				CollisionManager::resolveCollision(followerA, *it, collisionData);
			}
		}

		if (CollisionManager::entityCircleInterfaceDetection(followerA, leader, collisionData)) {
			CollisionManager::resolveCollision(followerA, leader, collisionData);
		}
	}
	
	
	//cout << getTileAt(Map::worldToGridPositionX(leader.physicsNode.getPosition().x), Map::worldToGridPositionY(leader.physicsNode.getPosition().y));
	int gridX = Map::worldToGridPositionX(leader.physicsNode.getPosition().x);
	int gridY = Map::worldToGridPositionY(leader.physicsNode.getPosition().y);
	Vector2 tileOrigin = Map::gridToWorldPosition(gridX, gridY);
	leader.setTile(tileOrigin, tileMap.getTileAt(gridX, gridY));

	for (auto it = terrainColliders.begin(); it != terrainColliders.end(); ++it) {
		if (CollisionManager::entityCircleInterfaceDetection(leader, *it, collisionData)) {
			CollisionManager::resolveCollision(leader, *it, collisionData);

			//find projection contact normal onto leader velocity
			//find component of leader velocity towards (actually away) from contact point
			float vecProjectionAwayFromContact = Vector2::Dot(collisionData.contactNormal, leader.velocityNode.getVelocity());

			
			if (vecProjectionAwayFromContact < 0.0f) {
				//if velocity is *towards* contact point
				//apply the opposite of component of velocity projected onto collison normal
				leader.velocityNode.applyVelocity(collisionData.contactNormal, vecProjectionAwayFromContact * 30.0f);
			}

			leader.leaderControler.colliding = true;
		}

		if (CollisionManager::entityCircleInterfaceDetection(dragon, *it, collisionData)) {
			CollisionManager::resolveCollision(dragon, *it, collisionData);
		}
	}

	leader.update(msec);

	dragon.setObstacles(&terrainColliders);
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