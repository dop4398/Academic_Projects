#pragma once
#include "stdafx.h"

class Node
{
private:
	bool visited;
	int xCoord;
	int yCoord;
	//static int tileSize = 50;
	Node* parent;
	//string texture;
	int hValue;
	int gValue;
	vector<Node*> neighbors;

public:
	Node(int row, int col);
	~Node();
	void Reset();
	void AddNeighbor(Node* neighbor);
	bool isPath;
	bool printed;

	// Getters and Setters
	bool GetVisited();
	void SetVisited(bool v);
	Node* GetParent();
	void SetParent(Node* p);
	int GetgValue();
	void SetgValue(int g);
	int GethValue();
	void SethValue(int h);
	int GetfValue();
	vector<Node*> GetNeightbors();
	int GetXCoord();
	int GetYCoord();
};

