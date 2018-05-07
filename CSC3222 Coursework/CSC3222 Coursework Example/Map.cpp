#include "Map.h"

const float Map::MAP_IMAGE_HALF_WIDTH = 864.0f;
const float Map::MAP_IMAGE_HALF_HEIGHT = 540.0f;
//these are manually chosen to match the map size defined above
const int Map::GRID_SIZE = 27;
const int Map::TILES_X = 59;
const int Map::TILES_Y = 38;
const Vector2 Map::MAP_IMAGE_BORDER_SIZE = Vector2(39.0f, 34.0f);

const Vector2 Map::MAP_BOTTOM_LEFT = Vector2(-MAP_IMAGE_HALF_WIDTH + MAP_IMAGE_BORDER_SIZE.x, -MAP_IMAGE_HALF_HEIGHT + MAP_IMAGE_BORDER_SIZE.y);

TileType Map::charToTile(char charTile){
	TileType tile;
	switch (charTile) {
	case 'f':
		tile = FLOOR;
		break;
	case 'w':
		tile = WALL;
		break;
	case 'r':
		tile = RUBBLE;
		break;
	case 'b':
		tile = BOULDER;
		break;
	case 'h':
		tile = HOARD;
		break;
	case 'p':
		tile = POOL;
		break;
	default:
		tile = INVALID_TILE;
	}

	return tile;

	
}
