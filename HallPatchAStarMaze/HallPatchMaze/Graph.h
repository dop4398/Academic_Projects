#pragma once
#include "PriorityQueue.h"

class Graph
{
private:
	vector<Node*> nodes;
	PriorityQueue* openList;
	vector<Node*> closedList;
	vector<Node*> obstacles;
	Node* currentNode;
	Node* startNode;
	int startIndex;
	Node* endNode;
	int endIndex;
	int gridWidth;
	int gridHeight;
	int** map;

public:
	Graph(int width, int height, int** map);
	~Graph();
	bool AStarAlgorithm();
	void ShortestPath();
	//void SetTextures();
	void SetStart(int x, int y);
	void SetEnd(int x, int y);
	Node* GetCurrentNode();
	Node* GetStartNode();
	Node* GetEndNode();
	vector<Node*> GetNodes();
	PriorityQueue* GetOpenList();
};

