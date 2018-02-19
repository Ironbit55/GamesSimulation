#pragma once
#include "../Entity.h"
class Dragon :
	public Entity
{
public:
	Dragon(const float rotation = 0.0f) : Entity(25.0f, 50.0f, 50.0f, -199.5f, rotation) {}

	Dragon(const Vector2 pos, const float rotation = 0.0f) : Dragon(rotation){
		this->physicsNode.setPosition(pos);
	}
	Dragon(const int gridX, const int gridY, const float rotation = 0.0f) : Dragon(rotation){
		this->setGridPosition(gridX, gridY);
	};
	~Dragon() {}
};

