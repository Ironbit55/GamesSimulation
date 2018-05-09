#pragma once
#include "Components/PhysicsNode.h"
#include "Components/VelocityNode.h"
#include <vector>
#include "Entity.h"

/*
 * based of: https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-movement-manager--gamedev-4278
 */
struct MovementNode{
	PhysicsNode& physicsNode;
	VelocityNode&  velocityNode;
};

class MovementManager
{
public:
	MovementManager(VelocityNode& host, CollidableNode& hostCollider, float maxForce = 100.0f, float lookahead = 100.0f) : 
	host(host), hostCollider(hostCollider), steeringForce(), maxForce(maxForce), MAX_LOOKAHEAD(lookahead) {};
	~MovementManager();
	MovementManager(const MovementManager& src) : host(src.host), hostCollider(src.hostCollider), maxForce(src.maxForce), MAX_LOOKAHEAD(src.MAX_LOOKAHEAD) {}

	MovementManager operator=(const MovementManager& rhs){
		return MovementManager(rhs);
	}


	void update();
	void setMaxForce(float maxForce){
		this->maxForce = maxForce;
	}

	void applySeek(Vector2 target, float arrivalRadius = 0);
	void applyFlee(Vector2 target);
	void applyEvade(VelocityNode target);
	void applyObstacleAvoidance();
	void addObstacles(std::vector<Entity>* obstacles){
		this->obstacles = obstacles;
	}


protected:
	VelocityNode& host;
	CollidableNode& hostCollider;
	Vector2 steeringForce;
	float maxForce;
	std::vector<Entity>* obstacles;
	const float MAX_LOOKAHEAD;

	Vector2 seek(Vector2 target, float arrivalRadius = 0);
	Vector2 flee(Vector2 target);
	Vector2 evade(VelocityNode target);
	Vector2 obstacleAvoidance();
	Vector2 obstacleAvoidanceRaycast(Vector2 ahead, Vector2 ahead2);


	void addSteeringForce(Vector2 force, float weight = 1.0f){
		steeringForce += force * weight;
	}

	bool lineCircleIntersection(Vector2 ahead, Vector2 ahead2, Entity& obstacle, CollisionData& outCollisionData);
	Entity* findNearestObstacle(Vector2 hostPosition, Vector2 ahead, Vector2 ahead2, CollisionData& outCollisionData);

};

