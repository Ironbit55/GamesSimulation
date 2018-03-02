#include "CollisionManager.h"



CollisionManager::CollisionManager(){
}


CollisionManager::~CollisionManager()
{
}

bool CollisionManager::circleCircleCollision(PhysicsNode& c1, PhysicsNode& c2){
	//cout << "circle-circle collision check" << "\n";
	
	
	Vector2 originDistance = c2.getPosition() - c1.getPosition();


	//there is a collision if distance between circle origins is less than the sum of their radiuses
	return originDistance.Length() < (c1.getBoundingCircle() + c2.getBoundingCircle());
	
}

bool CollisionManager::entityCircleInterfaceDetection(Entity& e1, Entity& e2, CollisionData& collisionDataOut){
	//check collidables here
	if (e1.collidableNode.isFixed && e2.collidableNode.isFixed) {
		return false;
	}

	Vector2 originDistance = e2.physicsNode.getPosition() - e1.physicsNode.getPosition();

	float penetrationDepth = (e1.collidableNode.boundingRadius + e2.collidableNode.boundingRadius) - originDistance.Length();

	//on no collision
	//we don't bother changing collisionDataOut at all...
	//is this right?
	if (penetrationDepth < 0.0f) {	//have some floating point tolerence here?
		return false;
	}

	//there is an overlap so fill up collision data

	collisionDataOut.contactNormal = Vector2::Normalise(originDistance);
	collisionDataOut.contactPoint = e1.physicsNode.getPosition() + (collisionDataOut.contactNormal * e2.collidableNode.boundingRadius);
	collisionDataOut.peneterationDepth = penetrationDepth;

	return true;
}

//bool CollisionManager::squareSquareCollision(Square* s1, Square* s2){
//	//cout << "square square collision check" << "\n";
//	//if s1 bottom edge is higher than s2 top ege then no collision
//	//for us higher means the y value will be SMALLER
//	if(s1->getBottomLeft().getY() < s2->getTopLeft().getY()){
//		return false;
//	}
//
//	//if s1 top edge is lower than s2 bottom edge then no collision
//	//lower means y will be GREATER
//	if(s1->getTopLeft().getY() > s2->getBottomLeft().getY())
//	{
//		return false;
//	}
//
//	//if s1 left edge is to the right of s2 right edge then no collision
//	//this means x value will be GREATER
//	if(s1->getTopLeft().getX() > s2->getTopRight().getX()){
//		return false;
//	}
//
//	//if s1 right edge is to the left of s2 left edge then no collision
//	//this means c value will be SMALLER
//	if(s1->getTopRight().getX() < s2->getTopLeft().getX()){
//		return false;
//	}
//
//	//if non of these things we have a collison
//	return true;
//}
//
////axis aligned rectangle circle collision
////but simlified for a square
////this might no handle the case where circle is inside square or other way round
////source: https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
////from e.James's answer
//bool CollisionManager::circleSquareCollision(Circle* c1, Square* s1){
//	//cout << "circle-square collision check" << "\n";
//	//we do this to find the point of the rectangle closest to the circle.
//	//this way we can reduce our problem to focus on one corner of the rectangle which
//	//may collide
//	Vector3D circleDistance = c1->getPosition() - s1->getCentre();
//	double circleDistanceX = abs(circleDistance.getX());
//	double circleDistanceY = abs(circleDistance.getY());
//
//	double s1HalfSize = s1->getSize() / 2;
//	//these two ifs check if the
//	//circle is far enough away from rectangle that no intersection is possible
//	if(circleDistanceX > (s1HalfSize + c1->getSize())){
//		return false;
//	}
//
//	if(circleDistanceY > (s1HalfSize / 2 + c1->getSize())){
//		return false;
//	}
//
//	//checks the case where circle if close enough that intersection is guranteed
//	if(circleDistanceX <= s1HalfSize){
//		return true;
//	}
//	if(circleDistanceY <= s1HalfSize){
//		return true;
//	}
//
//	//now we check whether the circle may intersect the corner of the rectangle
//
//	//calculate distance between centre of circle and corner
//	double cornerDistanceSquare = pow((circleDistanceX - s1HalfSize), 2)
//		+ pow(circleDistanceY - s1HalfSize, 2);
//	
//	//if this distance is less than the radius of the circle there is an intersection
//	return (cornerDistanceSquare <= pow(c1->getSize(), 2));
//}
