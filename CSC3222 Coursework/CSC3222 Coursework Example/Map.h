#pragma once
#include "../../nclgl/Vector2.h"

class Map{
	public:
		static const int GRID_SIZE;
		static const Vector2 MAP_IMAGE_BORDER_SIZE;
		static const float MAP_IMAGE_HALF_WIDTH;
		static const float MAP_IMAGE_HALF_HEIGHT;
		static const float MAP_WIDTH;
		static const float MAP_HEIGHT;
		static const Vector2 MAP_BOTTOM_LEFT;
		

		static Vector2 gridToWorldPosition(const int gridX, const int gridY){
			return Vector2((gridX * Map::GRID_SIZE) + MAP_BOTTOM_LEFT.x, (gridY * Map::GRID_SIZE) + MAP_BOTTOM_LEFT.y);
		}

	private:
		
};

