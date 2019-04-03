#include "stdafx.h"
#include "Node.h"


// Constructor - takes in the x and y coordinates for the node.
Node::Node(int row, int col)
{
	xCoord = row;
	yCoord = col;
	Reset();
}

Node::~Node()
{
}

// Resets the shortest distance and parent values.
void Node::Reset()
{
	gValue = INT32_MAX;
	parent = nullptr;
	isPath = false;
	printed = false;
	visited = false;
	neighbors = vector<Node*>();
}

// Adds a neighbor to the vector of neighbor nodes.
void Node::AddNeighbor(Node* neighbor)
{
	neighbors.push_back(neighbor);
}


// Getters and Setters
bool Node::GetVisited()
{
	return visited;
}
void Node::SetVisited(bool v)
{
	visited = v;
}

Node* Node::GetParent()
{
	return parent;
}
void Node::SetParent(Node* p)
{
	parent = p;
}

int Node::GetgValue()
{
	return gValue;
}
void Node::SetgValue(int g)
{
	gValue = g;
}

int Node::GethValue()
{
	return hValue;
}
void Node::SethValue(int h)
{
	hValue = h;
}

int Node::GetfValue()
{
	return gValue + hValue;
}

vector<Node*> Node::GetNeightbors()
{
	return neighbors;
}

int Node::GetXCoord()
{
	return xCoord;
}

int Node::GetYCoord()
{
	return yCoord;
}
