#pragma once
#include "../../nclgl/CubeRobot.h"

class TileMap
{
public:
	TileMap();
	~TileMap();

	void setTileMap() {};

	TileType getTileAt(int x, int y) {
		if (x >= 0 && y >= 0 && x < Map::TILES_X && y < Map::TILES_Y) {
			int flippedY = Map::TILES_Y - y;
			return tileMap[flippedY * Map::TILES_X + x];
		}
		return INVALID_TILE;


	}

	bool isWalkable(int x, int y){
		switch (getTileAt(x, y)) {
			case TileType::FLOOR: return true;
			case TileType::RUBBLE: return true;
			case TileType::HOARD: return true;
			case TileType::POOL: return true;
			default: return false;
		}
	}

	int getTileWeight(int x, int y){
		switch (getTileAt(x, y)) {
			case TileType::FLOOR: return 5;
			case TileType::RUBBLE: return 2;
			case TileType::HOARD: return 5;
			case TileType::POOL: return 5;
			default: return 999;
		}
	}


protected:
	TileType tileMap[Map::TILES_X *  Map::TILES_Y];

};

