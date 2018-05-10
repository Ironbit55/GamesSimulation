#include "TileMap.h"



TileMap::TileMap() {
	//fill up map from file
	ifstream mapfile("tileMap.txt");;
	string empty;
	if (!mapfile.is_open()) {
		cout << "Unable to open file";
		return;
	}


	for (int y = 0; y < Map::TILES_Y; y++) {
		for (int x = 0; x < Map::TILES_X; x++) {
			char tile;
			mapfile >> tile;

			TileType tileType = Map::charToTile(tile);
			tileMap[(y * Map::TILES_X) + x] = tileType;
		}

		getline(mapfile, empty);
	}
}


TileMap::~TileMap()
{
}
