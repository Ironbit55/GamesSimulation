#pragma once
#include "Map.h"
#include "TileMap.h"
#include <queue>


const int MAX_NUM_EDGES = 8;
struct Edge{
	int id;
	int cost;
	int heuristic;
};
struct Node{
	int id;
	int edgeCount;
	Edge edges[MAX_NUM_EDGES];
	Vector2 position;
	bool walkable;
	double priority;





	bool operator< (const Node& rhs) {
		return this->priority < rhs.priority;
	}

};

class Pathfinding
{
public:
	Pathfinding(TileMap& tileMap) : tileMap(tileMap) {};
	~Pathfinding() {};

	void fillGraphFromGrid();
	void setWalkable(int nodeId, bool walkable);
	void setGridNodeObstructed(int x, int y);
	void setGridNodeDefaultWalkable(int x, int y);
	int getGridNodeId(int x, int y){
		return Map::getIndex(x, y);
	}
	


	/*
	 * based off of: https://www.redblobgames.com/pathfinding/a-star/implementation.html
	 */
	bool aStarSearch(int startNodeId, int endNodeId, vector<Node>& nodes) {
		std::priority_queue<Node*, std::vector<Node*>> openQueue;
		//index is id of node
		int* cameFrom = new int[graph.size()];
		double* costSoFar = new double[graph.size()];
		for (int i = 0; i < graph.size(); ++i) {
			cameFrom[i] = -1;
			costSoFar[i] = -1;
		}

		openQueue.push(&graph.at(startNodeId));
		while (!openQueue.empty()) {
			Node* currentNode = openQueue.top();
			//remove from open list
			openQueue.pop();

			if (currentNode->id == endNodeId) {
				//found end
				break;
			}

			for (int i = 0; i < currentNode->edgeCount; ++i) {
				double newCost = costSoFar[currentNode->id] + currentNode->edges[i].cost;
				if (costSoFar[currentNode->edges[i].id] < 0 || newCost < costSoFar[currentNode->edges[i].id]) {
					//cost hasn't been recorded for this not yet, or the new cost is better
					//so add to closed list
					costSoFar[currentNode->edges[i].id] = newCost;
					double priority = currentNode->edges[i].cost + manhattanHeuristic(*currentNode, graph.at(endNodeId));
					openQueue.push(&graph.at(currentNode->edges[i].id));
					//so we can retrace our steps
					cameFrom[currentNode->edges[i].id] = currentNode->id;
				}
			}

		}

		nodes.clear();

		/*int next = cameFrom[endNodeId];
		while(next != -1 && next != startNodeId){
		nodes.push_back(graph.at(next));
		next = cameFrom[next];
		}*/

		//get path by retracing steps

		//start at end node, retrace to start node
		int next = -1;
		for (next = endNodeId; next != -1 && next != startNodeId; next = cameFrom[next]) {
			nodes.push_back(graph.at(next));
		}

		if (next == -1) {
			return false;
		} else {
			nodes.push_back(graph.at(next));
			std::reverse(nodes.begin(), nodes.end());
			return true;
		}

		//get path by retracing steps

	}

protected:
	TileMap tileMap;
	vector<Node> graph;

	Node& getNode(int id){
		return graph.at(id);
	}

	Node& gridToNode(int x, int y);
private:
	int gridGetNeighbour(int gridIndex, int neighbourIndex) {
		if (neighbourIndex < 0 || neighbourIndex > 7) {
			return -1;
		}
		//012
		//3x4
		//567
		if (neighbourIndex > 3) {
			neighbourIndex++;
		}

		return ((gridIndex % 3) + neighbourIndex % 3) + ((gridIndex / 3) + (neighbourIndex / 3) * Map::TILES_X);
	}

	/*
	 *
	 */
	void setGridEdges(int middleX, int middleY, Node& node){
		Edge edges[8];
		node.edgeCount = 0;
		int count = 0;
		for (int y = middleY - 1; y <= middleY + 1; ++y) {
			for (int x = middleX - 1; x <= middleX + 1; ++x) {
				if(x != middleX || y != middleY){
					if (tileMap.isWalkable(x, y)) {

						//Manhattan cost
						int manhattanCost = abs(x - middleX) + abs(y - middleY);
						int diaganalCost = 10;
						if(x != middleX && y != middleY){
							//diagonal edge
							diaganalCost = 14;
						}
						int weight = tileMap.getTileWeight(x, y);
						//scale this by terrain cost
						node.edges[count].id = Map::getIndex(x, y);
						node.edges[count].cost = weight * diaganalCost;
						//precompute heuristic cus we can
						//wait we can't that was a lie
						node.edges[count].heuristic = manhattanCost;
						node.edgeCount++;
						count++;
					}
				}
			}
		}
		
	}

	

	double manhattanHeuristic(Node nodeA, Node nodeB){
		//alternatively could use id to get grid x, y and compare that
		return abs(nodeA.position.x - nodeB.position.x) + abs(nodeA.position.y - nodeB.position.x);
	}

	
};

