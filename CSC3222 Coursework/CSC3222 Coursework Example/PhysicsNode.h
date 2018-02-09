#pragma once
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector2.h"
#include "Map.h"
/* An elementary Physics Data structure, containing the bare minimum of information necessary to
say where an object should appear, and which way it should point. As we increase the complexity
of our physics system, we will add elements to this structure, but the elements which can affect
the graphical representation of entities are present below (and integrated with the graphics
framework already, to make your life easier). Ultimately, you may decide to replace this data
structure entirely (for example with a class to allow inclusion of embedded functions) but be
aware that doing so might require some slight doctoring of Renderer::Renderer() and
SceneNode::Update() */

struct PhysicsData {
	Vector3 position;
	float rotation;
	Vector3 scale;
};

class PhysicsNode
{
public:

	PhysicsNode(const Vector2 _position = Vector2(0.0f, 0.0f), const float _depth = 0.0f, const float _rotation = 0.0f,
		const Vector3 _scale = Vector3(1.0f, 1.0f, 1.0f))
			: position(_position),
			depth(_depth), 
			rotation(_rotation), 
			scale(_scale) {};


	PhysicsNode(int gridX, int gridY, const float _rotation = 0.0f, 
		const Vector3 _scale = Vector3(1.0f, 1.0f, 1.0f)) 
			: position(gridToWorldPosition(gridX, gridY)),
			depth(depth),
			rotation(_rotation),
			scale(_scale) {}
		
	

	Vector2 const getPosition() { return position; }
	Vector3 getPosition3d() { return Vector3(position.x, position.y, depth); }
	float const getPositionX() { return position.x; }
	float const getPositionY() { return position.y; }
	float const getDepth() { return depth; }
	float const getRotation() { return rotation; }
	Vector3 const getScale() { return scale; }
	
	Vector2 gridToWorldPosition(int gridX, int gridY);


	void setPosition(Vector2 position) { this->position = position; }
	
	void moveX(float deltaX) { this->position.x += deltaX; }
	void moveY(float deltaY) { this->position.y += deltaY; }
	
	void move(Vector2 delta) { this->position += delta; }

	void setDepth(const float depth) { this->depth = depth; }
	void setRotation(float rotation) { this->rotation = rotation; }
	void updateRotation(float delta) { this->rotation += delta; }
	void setScale(Vector3 scale) {this->scale = scale;}
	void setScaleXY(float scaleX, float scaleY) 
	{ 
		this->scale.x = scaleX;
		this->scale.y = scaleY;
	}
	void addToScaleXY(float deltaX, float deltaY) {
		this->scale.x += deltaX;
		this->scale.y += deltaY;
	}

	void setGridPosition(int x, int y) { 
		setPosition(gridToWorldPosition(x, y));
	}

	~PhysicsNode() {};

private:
	//PhysicsData physicsData;
	Vector2 position;
	float depth;
	float rotation;
	Vector3 scale;
};

