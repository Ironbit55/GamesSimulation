#pragma once
#include "PhysicsNode.h"
#include <vector>
#include "../../../nclgl/quaternion.h"

void PhysicsNode::setRotation(Vector2 dir){
	//we can find the angle and construct a rotation quaternion using the up axis as rotation axis

	
	//get the angle
	float angle = -atan2(dir.x, dir.y); //the minus here seems to make it work right with our coordinate system

	//could also construct rotation matrix using dir, up and cross product to get right then convert matrix to quaternion
	//but they have the same result so I guess the math does the same thing

	qrotation = Quaternion::AxisAngleRadsToQuaternion(Vector3(0, 0, 1), angle);
}