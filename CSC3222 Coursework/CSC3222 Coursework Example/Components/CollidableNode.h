#pragma once
//this should just be a struct really
class CollidableNode
{
public:
	CollidableNode() : boundingRadius(0.0f), elasticity(1.0f), isFixed(false) {};
	CollidableNode(float boundingRadius, bool isFixed, float elasticity = 1.0f) : boundingRadius(boundingRadius), elasticity(elasticity), isFixed(isFixed) {};
	~CollidableNode() {};

	float boundingRadius;
	float elasticity;
	bool isFixed;
};

