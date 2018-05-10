#include "Pathfinding.h"


void Pathfinding::fillGraphFromGrid() {
	for (int y = 0; y < Map::TILES_Y; ++y) {
		for (int x = 0; x < Map::TILES_X; ++x) {
			//add collider based on tile
			TileType tileType = tileMap.getTileAt(x, y);
			
			//set node
			Node node;
			node.id = Map::getIndex(x, y);
			node.position = Map::gridToWorldPosition(x, y);
			node.walkable = tileMap.isWalkable(x, y);
			node.priority = 0.0f;
			setGridEdges(x, y, node);
			
			graph.push_back(node);

		}
	}

}

void Pathfinding::setGridNodeObstructed(int x, int y) {
	gridToNode(x, y).walkable = false;
}

void Pathfinding::setGridNodeDefaultWalkable(int x, int y) {
	gridToNode(x, y).walkable = tileMap.isWalkable(x, y);
}

Node& Pathfinding::gridToNode(int x, int y) {
	return graph.at(Map::getIndex(x, y));
}
