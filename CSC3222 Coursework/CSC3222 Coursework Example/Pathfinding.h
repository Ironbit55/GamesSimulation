#pragma once
#include "Map.h"
#include "TileMap.h"
#include <queue>
#include <functional>


const int MAX_NUM_EDGES = 8;
struct Edge{
	int id;
	int distanceCost;
	int weight;
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

struct CompNode {
	bool operator()(Node* n1, Node* n2) {
		return n1->priority > n2->priority;
	}

};

//<priority,node>
typedef std::pair<double, Node*> QueueElement;

struct CompQueueElement {
	//bool operator()(Node* n1, Node* n2) {
	//	return n1->priority > n2->priority;
	//}

	bool operator()(QueueElement& n1, QueueElement& n2) {
		return n1.first > n2.first;
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
	int getGridNodeId(Point grid){
		return getGridNodeId(grid.x, grid.y);
	}
	


	/*
	 * based off of: https://www.redblobgames.com/pathfinding/a-star/implementation.html
	 */
	bool aStarSearch(int startNodeId, int endNodeId, vector<Node>& nodes, bool shortestPath = false) {
		//we need to use a paur to map a node to its latest priority value
		//in this way we can replace existing nodes by replacing their priority value
		std::priority_queue<QueueElement, std::vector<QueueElement>, CompQueueElement> openQueue;
		//index is id of node
		//basiicaly the closed list
		int* cameFrom = new int[graph.size()];
		double* costSoFar = new double[graph.size()];

		//initialse to < 0, so we know when an item hasn't been set
		for (int i = 0; i < graph.size(); ++i) {
			cameFrom[i] = -1;
			costSoFar[i] = 0;
		}
		int count = 0;
		//openQueue.push(&graph.at(startNodeId));
		openQueue.emplace(0, &graph.at(startNodeId));
		while (!openQueue.empty()) {
			Node* currentNode = openQueue.top().second;
			//remove from open list
			openQueue.pop();

			if (currentNode->id == endNodeId) {
				//found end
				break;
			}
			count++;
			for (int i = 0; i < currentNode->edgeCount; ++i) {
				Node* next = &graph.at(currentNode->edges[i].id);
				double newCost = costSoFar[currentNode->id] + getCost(currentNode->edges[i], shortestPath);
				//if (costSoFar[currentNode->edges[i].id] < 0){
				//	newCost = getCost(currentNode->edges[i], shortestPath);
				//}
				
				if (costSoFar[next->id] < 1 || newCost < costSoFar[next->id]) {
					
					//cost hasn't been recorded for this node yet or the new cost is better
					//so add to closed list
					costSoFar[next->id] = newCost;
					double priority = newCost + manhattanHeuristic(*next, graph.at(endNodeId));
					
					//add or replace node priority value
					openQueue.emplace(priority, next);
					//openQueue.push(next);
					//so we can retrace our steps
					cameFrom[next->id] = currentNode->id;
				}
			}

		}

		nodes.clear();

		//get path by retracing steps

		//start at end node, retrace to start node
		int next = -1;
		for (next = endNodeId; next != -1 && next != startNodeId; next = cameFrom[next]) {
			nodes.push_back(graph.at(next));
		}

		delete[] cameFrom;
		delete[] costSoFar;
		if (next == -1) {
			return false;
		} else {
			//need to add start node
			nodes.push_back(graph.at(next));
			//reverse so path is in order of start to end
			std::reverse(nodes.begin(), nodes.end());
			return true;
		}

		

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

	double getCost(Edge& edge, bool shortestPath = false){
		if(shortestPath){
			return edge.distanceCost;
		}
		return edge.distanceCost * edge.weight;
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
						int distanceCost = 10;
						if(x != middleX && y != middleY){
							//diagonal edge
							distanceCost = 14;
							if (!tileMap.isWalkable(x - (x - middleX), y) || !tileMap.isWalkable(x, y - (y - middleY))){
								//to be able to walk diagonally, both neighbours of diagonal must be walkable
								continue;;
							}
						}
						int weight = tileMap.getTileWeight(x, y);
						//scale this by terrain cost
						node.edges[count].id = Map::getIndex(x, y);
						node.edges[count].distanceCost = distanceCost;
						node.edges[count].weight = weight;
						//precompute heuristic cus we can
						//wait we can't that was a lie
						//node.edges[count].heuristic = manhattanCost;
						node.edgeCount++;
						count++;
					}
				}
			}
		}
		
	}

	

	double manhattanHeuristic(Node nodeA, Node nodeB){
		//alternatively could use id to get grid x, y and compare that
		return abs(nodeA.position.x - nodeB.position.x) + abs(nodeA.position.y - nodeB.position.y);
	}

	
};

