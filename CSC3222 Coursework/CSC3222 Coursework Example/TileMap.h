#pragma once
#include "../../nclgl/CubeRobot.h"
struct Point {
	Point(int x, int y) : x(x), y(y) {};
	int x;
	int y;	
};

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
			case TileType::FLOOR: return 10;
			case TileType::RUBBLE: return 25;
			case TileType::HOARD: return 10;
			case TileType::POOL: return 10;
			default: return 999;
		}
	}

	Point getPoolTile(){
		return Point(45, 20);
	}

	Point getHoardTile() {
		return Point(25, Map::TILES_Y - 4);
	}

	Point posToGrid(Vector2 pos) {
		return Point(Map::worldToGridPositionX(pos.x), Map::worldToGridPositionY(pos.y));
		
	}



protected:
	TileType tileMap[Map::TILES_X *  Map::TILES_Y];

};

