#pragma once
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector2.h"
#include "../Map.h"
#include "../../../nclgl/quaternion.h"

/* An elementary Physics Data structure, containing the bare minimum of information necessary to
say where an object should appear, and which way it should point. As we increase the complexity
of our physics system, we will add elements to this structure, but the elements which can affect
the graphical representation of entities are present below (and integrated with the graphics
framework already, to make your life easier). Ultimately, you may decide to replace this data
structure entirely (for example with a class to allow inclusion of embedded functions) but be
aware that doing so might require some slight doctoring of Renderer::Renderer() and
SceneNode::Update() */

struct CollisionData {
	Vector2 contactPoint;
	Vector2 contactNormal;
	float peneterationDepth;
};

struct PhysicsData {
	Vector3 position;
	float rotation;
	Vector3 scale;
};

class PhysicsNode {
public:

	PhysicsNode(const Vector3 _scale = Vector3(100.0f, 100.0f, 100.0f), const float _depth = 0.0f, const float _rotation = 0.0f)
		: position(Vector2(0.0f, 0.0f)),
		depth(_depth),
		scale(_scale),
		colour(1, 1, 1, 0.999) {
		setRotation(_rotation);
	};

	PhysicsNode(const Vector2 _position, const Vector3 _scale, const float _depth = 0.0f, const float _rotation = 0.0f) : PhysicsNode(_scale, _depth, _rotation){
		setPosition(_position);
	};
			
		
	

	Vector2 getPosition() const { return position; }
	Vector3 getPosition3d() const { return Vector3(position.x, position.y, depth); }
	float getPositionX() const { return position.x; }
	float getPositionY() const { return position.y; }
	float getDepth() const { return depth; }
	Vector3 getScale() const { return scale; }
	Quaternion getQrotation() const { return qrotation; }
	Vector2 getForwardDirection() const{
		//default foward is up in y
		Vector3 forward = Vector3(0, 1, 0);

		//rotate foward direction by rotation quaternion
		Quaternion::RotatePointByQuaternion(getQrotation(), forward);
		//foward now points in direction we are facing
		
		return Vector2(forward.x, forward.y);
	}

	
	//Vector2 gridToWorldPosition(int gridX, int gridY);


	void setPosition(const Vector2 position) { this->position = position; }
	
	void moveX(const float deltaX) { this->position.x += deltaX; }
	void moveY(const float deltaY) { this->position.y += deltaY; }
	
	void move(const Vector2 delta) { this->position += delta; }

	void setDepth(const float depth) { this->depth = depth; }
	void setRotation(const float rotation){
		this->qrotation = Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 1), rotation);
}
	void setRotation(const Vector2 dir);


	void updateRotation(const float delta) { this->qrotation = this->qrotation * Quaternion::AxisAngleToQuaternion(Vector3(0, 0, 1), delta); }
	void setScale(const Vector3 scale) {this->scale = scale;}
	void setScaleXY(const float scaleX, const float scaleY){ 
		this->scale.x = scaleX;
		this->scale.y = scaleY;
	}
	void addToScaleXY(const float deltaX, const float deltaY) {
		this->scale.x += deltaX;
		this->scale.y += deltaY;
	}

	float getBoundingCircle() const { return boundingCircleRadius; }

	void setBoundingCircle(const float radius){
		boundingCircleRadius = radius;
	}



	~PhysicsNode() {};

	//yeah I'm not convinced this should go here but... whatever
	Vector4 colour;
private:
	//PhysicsData physicsData;
	Vector2 position;
	float depth;
	Quaternion qrotation;
	Vector3 scale;
	float boundingCircleRadius;
};

