#include "MovementManager.h"
#include "CollisionManager.h"


MovementManager::~MovementManager() {
}

void MovementManager::update() {
	//limit steering force to maxForce
	if (steeringForce.Length() > 0.0f) {
		if (steeringForce.Length() > maxForce) {
			steeringForce = steeringForce* (maxForce / steeringForce.Length());
		}

		host.applyForce(steeringForce);
		
	}
	steeringForce = Vector2();
}

void MovementManager::applySeek(Vector2 target, float arrivalRadius) {
	addSteeringForce(seek(target, arrivalRadius), 5.0f);
}

void MovementManager::applyFlee(Vector2 target) {
	addSteeringForce(flee(target));
}

void MovementManager::applyEvade(VelocityNode& target) {
	addSteeringForce(evade(target));
}

void MovementManager::applyObstacleAvoidance() {
	addSteeringForce(obstacleAvoidance(), 5.0f);
}

void MovementManager::applyFollowPath(float pathRadius) {
	addSteeringForce(followPath(pathRadius), 6.0f);
}

void MovementManager::applyFollowLeader(VelocityNode& leader, float leaderSightDistance, float leaderBehindDistance){
	addSteeringForce(followLeader(leader, leaderSightDistance, leaderBehindDistance), 10.0f);
}



Vector2 MovementManager::seek(Vector2 target, float arrivalRadius) {
	Vector2 desiredVelocity = target - host.getPosition();
	float distance = desiredVelocity.Length();
	desiredVelocity.Normalise();

	if(distance <= arrivalRadius) {
		//do arrival smoothing
		desiredVelocity = desiredVelocity * (host.getMaxSpeed() * (distance / arrivalRadius));
		
	}else {
		desiredVelocity = desiredVelocity * host.getMaxSpeed();
	}

	Vector2 seekForce = desiredVelocity - host.getVelocity();
	return seekForce;
}

Vector2 MovementManager::flee(Vector2 target) {
	Vector2 desiredVelocity = Vector2::Normalise(host.getPosition() - target);
	//change to maxForce
	desiredVelocity = desiredVelocity * host.getMaxSpeed();

	Vector2 fleeForce = desiredVelocity - host.getVelocity();

	return fleeForce;
}

Vector2 MovementManager::evade(VelocityNode& target) {
	Vector2 distance = target.getPosition() - host.getPosition();
	int updatesAhead = distance.Length() / host.getMaxSpeed();

	Vector2 targetFuturePosition = (target.getPosition() + target.getVelocity()) * updatesAhead;
	return flee(targetFuturePosition);
}

Vector2 MovementManager::obstacleAvoidance() {
	float lineStartRatio = 0.00;
	float dynamicLookahead = (host.getVelocity().Length() / host.getMaxSpeed()) * MAX_LOOKAHEAD;
	bool avoid = false;
	CollisionData obstacleCollisionData;
	//raycast in front
	Vector2 lineEnd = host.getPosition() + (Vector2::Normalise(host.getVelocity()) * dynamicLookahead);
	Vector2 lineStart = host.getPosition() + (Vector2::Normalise(host.getVelocity()) * dynamicLookahead * lineStartRatio);
	

	Vector2 obstacleAvoidanceForce = Vector2();
	Vector2 frontForce = obstacleAvoidanceRaycast(lineStart, lineEnd);
	if(frontForce.Length() > 0){
		obstacleAvoidanceForce += (frontForce * 1.5f);
		host.getPhysicsNode()->colour = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		avoid = true;
		//return frontForce;
	}

	

	//raycast front right
	Vector3 velocityRotatedRight = Matrix4::Rotation(-30, Vector3(0, 0, 1)) * Vector3(Vector2::Normalise(host.getVelocity()));
	velocityRotatedRight = velocityRotatedRight * 0.75f;

	lineEnd = host.getPosition() + (Vector2(velocityRotatedRight.x, velocityRotatedRight.y) * dynamicLookahead);
	lineStart = host.getPosition() + (Vector2(velocityRotatedRight.x, velocityRotatedRight.y) * dynamicLookahead * lineStartRatio);
	Vector2 rightForce = obstacleAvoidanceRaycast(lineStart, lineEnd);
	if(rightForce.Length() > 0){
		obstacleAvoidanceForce += (rightForce * 0.8f);
		host.getPhysicsNode()->colour = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		avoid = true;
		//return rightForce;
		
	}
	

	//raycast front left
	Vector3 velocityRotatedLeft = Matrix4::Rotation(30, Vector3(0, 0, 1)) * Vector3(Vector2::Normalise(host.getVelocity()));
	velocityRotatedLeft = velocityRotatedLeft * 0.75f;

	lineEnd = host.getPosition() + (Vector2(velocityRotatedLeft.x, velocityRotatedLeft.y) * dynamicLookahead);
	lineStart = host.getPosition() + (Vector2(velocityRotatedLeft.x, velocityRotatedLeft.y) * dynamicLookahead * lineStartRatio);
	Vector2 leftForce = obstacleAvoidanceRaycast(lineStart, lineEnd);
	if (leftForce.Length() > 0) {
		obstacleAvoidanceForce += (leftForce * 1);
		host.getPhysicsNode()->colour = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		avoid = true;
		//return leftForce;
	}

	if (!avoid) {
		host.getPhysicsNode()->colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	return obstacleAvoidanceForce;
}

Vector2 MovementManager::obstacleAvoidanceRaycast(Vector2 ahead, Vector2 ahead2) {
	CollisionData obstacleCollisionData;
	Entity* nearestObstacle = findNearestObstacle(host.getPosition(), ahead, ahead2, obstacleCollisionData);
	if (nearestObstacle == nullptr) {
		return Vector2();
	}


	Vector2 avoidForce = Vector2::Normalise(host.getPosition() - nearestObstacle->velocityNode.getPosition());
	Vector2 vecToObstacleContact = obstacleCollisionData.contactPoint - nearestObstacle->physicsNode.getPosition();
	vecToObstacleContact.Normalise();
	float vecProjectionAwayFromContact = Vector2::Dot(host.getVelocity(), avoidForce);
	vecProjectionAwayFromContact = -vecProjectionAwayFromContact;
	if (vecProjectionAwayFromContact < 0.0f) {
		//if velocity is away from centre then don't bother applying avoidance 
		vecProjectionAwayFromContact = 0;
	}
	return avoidForce * vecProjectionAwayFromContact * 10.0f;

}

Vector2 MovementManager::followLeader(VelocityNode& leader, float leaderSightDistance, float leaderBehindDistance) {
	Vector2 followForce;
	Vector2 hostV = leader.getVelocity();
	hostV.Normalise();

	//caldulate ahead point

	//default forward is up in y
	Vector2 aheadPos = leader.getPosition() + (hostV * leaderBehindDistance);
	
	//calculate behind point
	//can get vector point behind leader based on leaders rotation,
	//and scale this by velocity
	Vector3 behindVec = -leader.getPhysicsNode()->getForwardDirection();
	Vector2 behindPos = leader.getPosition() - (hostV * (leaderBehindDistance));
	behindPos += Vector2(behindVec.x, behindVec.y) * MIN_LEADER_BEHIND;

	//seek behind point
	followForce += seek(behindPos, Map::GRID_SIZE * 2);

	//evade ahead point
	if(Vector2::Distance(aheadPos, host.getPosition()) < leaderSightDistance || Vector2::Distance(leader.getPosition(), host.getPosition()) <= leaderSightDistance){
		//if host is in leaders line of sight apply evasion
		followForce += evade(leader) * 2.0f;
	}

	followForce += seperation() * 1.0f;
	//apply flock seperation
	return followForce;
}

Vector2 MovementManager::seperation() {
	Vector2 force;
	int numNeighbours = 0;
	//should sort by distance...
	//find all members of flock (excluding self) within radius
	//get some of vectors between host and neighbour
	for (auto it = flock.begin(); it != flock.end(); ++it){
		float distance = Vector2::Distance((*it)->velocityNode.getPosition(), host.getPosition());
		if (distance > 0.00001f && distance < SEPERATION_RADIUS){
			force += (*it)->velocityNode.getPosition() - host.getPosition();
			numNeighbours++;
		}else{
			int i = 0;
			i++;
		}
	}

	//get average difference vector
	//essentially finds a vector towards the centre of mass of the flock within radius
	if(numNeighbours != 0){ 
		force = force / numNeighbours;
		
	}

	force.Normalise();
	force = force * MAX_SEPERATION;
	//invert vector to point away from centre
	return -force;
}

Vector2 MovementManager::followPath(float pathRadius) {
	Vector2 force;
	if(!pathIsSet){
		return force;
	}

	Vector2 target = path[currentPathLocation];
	float distanceToTarget = Vector2::Distance(host.getPosition(), target);
	if(distanceToTarget <= pathRadius) {
		//if next target is final target, we want to make sure we get up really close to the target locations
		currentPathLocation += pathDirection;

		if (currentPathLocation < 0) {
			currentPathLocation = 0;
		} else if (currentPathLocation > (path.size() - 1)) {
			currentPathLocation = path.size() - 1;
		} 

		
		
	}

	return seek(target);
}

bool MovementManager::lineCircleIntersection(Vector2 ahead, Vector2 ahead2, Entity& obstacleEntity, CollisionData& outCollisionData) {

	CollisionData collisionData;

	if(CollisionManager::lineCircleCollision(ahead, ahead2, obstacleEntity.physicsNode.getPosition(), obstacleEntity.collidableNode.boundingRadius, collisionData)){
		outCollisionData = collisionData;
		return true;
	}

	if (CollisionManager::circleCircleCollision(host.getPosition(), hostCollider.boundingRadius, obstacleEntity.physicsNode.getPosition(), obstacleEntity.collidableNode.boundingRadius, collisionData)) {
		outCollisionData = collisionData;
		return true;
	} 

	return false;
	
}

Entity* MovementManager::findNearestObstacle(Vector2 hostPosition, Vector2 ahead, Vector2 ahead2, CollisionData& outCollisionData) {
	Entity* nearestObstacle = nullptr;
	
	Vector2 contactPointCandidate;
	CollisionData collisionDataCandidate;

	for (auto it = obstacles->begin(); it != obstacles->end(); ++it){
		bool collision = lineCircleIntersection(ahead, ahead2, *it, collisionDataCandidate);
		
		if (collision && (nearestObstacle == nullptr ||
			Vector2::Distance(hostPosition, it->physicsNode.getPosition()) < Vector2::Distance(hostPosition, nearestObstacle->physicsNode.getPosition()))) {
			//this obstacle is closer to host than current nearest obstacle
			//set this obstacle as nearest
			outCollisionData = collisionDataCandidate;
			nearestObstacle = &(*it);
			
		}
	}

	return nearestObstacle;
}
