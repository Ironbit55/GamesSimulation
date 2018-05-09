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
		steeringForce = Vector2();
	}
}

void MovementManager::applySeek(Vector2 target, float arrivalRadius) {
	addSteeringForce(seek(target, arrivalRadius), 5.0f);
}

void MovementManager::applyFlee(Vector2 target) {
	addSteeringForce(flee(target));
}

void MovementManager::applyEvade(VelocityNode target) {
	addSteeringForce(evade(target));
}

void MovementManager::applyObstacleAvoidance() {
	addSteeringForce(obstacleAvoidance(), 5.0f);
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

Vector2 MovementManager::evade(VelocityNode target) {
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
	Vector3 velocityRotatedRight = Matrix4::Rotation(-28, Vector3(0, 0, 1)) * Vector3(Vector2::Normalise(host.getVelocity()));
	velocityRotatedRight = velocityRotatedRight * 0.8f;

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
	Vector3 velocityRotatedLeft = Matrix4::Rotation(28, Vector3(0, 0, 1)) * Vector3(Vector2::Normalise(host.getVelocity()));
	velocityRotatedLeft = velocityRotatedLeft * 0.8f;

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



	Vector2 vecToObstacleContact = obstacleCollisionData.contactPoint - nearestObstacle->physicsNode.getPosition();
	vecToObstacleContact.Normalise();
	float vecProjectionAwayFromContact = Vector2::Dot(host.getVelocity(), obstacleCollisionData.contactNormal);
	float away = -1.0f;
	if (vecProjectionAwayFromContact < 0.0f) {
		//if velocity is away from centre then don't bother applying avoidance 
		vecProjectionAwayFromContact = 0;
		away = 0;
	}
	return  obstacleCollisionData.contactNormal  * obstacleCollisionData.peneterationDepth * -15.0f;

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
