#pragma once
#include "../../nclgl/Vector2.h"

class Map
{
	public:
		static const int GRID_SIZE;
		static const Vector2 MAP_OFFSET;

		static Vector2 gridToWorldPosition(int gridX, int gridY)
		{
			return Vector2((gridX * Map::GRID_SIZE) + MAP_OFFSET.x, (gridY * Map::GRID_SIZE) + MAP_OFFSET.y);
		}
};

