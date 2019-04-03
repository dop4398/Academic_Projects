#include "stdafx.h"
#include "Graph.h"

// Graph constructor - sets up the graph and all the nodes.
Graph::Graph(int width, int height, int** m)
{
	openList = new PriorityQueue();
	closedList = vector<Node*>();
	nodes = vector<Node*>();
	gridWidth = width;
	gridHeight = height;
	map = m;

	// Create the nodes for the grid
	for (int row = 0; row < gridHeight; row++)
	{
		for (int col = 0; col < gridWidth; col++)
		{
			nodes.push_back(new Node(row, col)); // new Node(row * Node.GetTileSize(), col * Node.GetTileSize())
		}
	}

	// Set up the neighbors
	for (Node* n1 : nodes)
	{
		// if this node is not a wall
		if (map[n1->GetXCoord()][n1->GetYCoord()] == 0)
		{
			for (Node* n2 : nodes)
			{
				// if this node is also not a wall
				if (map[n2->GetXCoord()][n2->GetYCoord()] == 0)
				{
					// If the x coordinates are 1 apart and the y is the same, set them as neighbors
					if (n1->GetXCoord() + 1 == n2->GetXCoord() && n1->GetYCoord() == n2->GetYCoord())
					{
						n1->AddNeighbor(n2);
					}
					if (n1->GetXCoord() - 1 == n2->GetXCoord() && n1->GetYCoord() == n2->GetYCoord())
					{
						n1->AddNeighbor(n2);
					}
					// Then do that but opposite
					if(n1->GetYCoord() + 1 == n2->GetYCoord() && n1->GetXCoord() == n2->GetXCoord())
					{
						n1->AddNeighbor(n2);
					}
					if (n1->GetYCoord() - 1 == n2->GetYCoord() && n1->GetXCoord() == n2->GetXCoord())
					{
						n1->AddNeighbor(n2);
					}
				}
			}
		}		
	}
}


Graph::~Graph()
{
	for (Node* n : nodes)
	{
		delete n;
	}
	delete openList;
}


// Algorithm for A*
bool Graph::AStarAlgorithm()
{
	if ((*openList->peek()).GetXCoord() != endNode->GetXCoord() || (*openList->peek()).GetYCoord() != endNode->GetYCoord())
	{
		currentNode = openList->dequeue();
		closedList.push_back(currentNode);

		for (Node* neighbor : currentNode->GetNeightbors())
		{
			int cost = currentNode->GetgValue() + 1;

			if (openList->contains(neighbor) && cost < neighbor->GetgValue())
			{
				openList->remove(neighbor);
			}

			bool inClosed = false;
			Node* itemToRemove = nullptr;
			int i = 0;
			int index;
			for (Node* nCl : closedList)
			{
				if (neighbor->GetXCoord() == nCl->GetXCoord() && neighbor->GetYCoord() == nCl->GetYCoord())
				{
					inClosed = true;
					itemToRemove = nCl;
					index = i;
				}
				i++;
			}
			// if the neighbor is on the closed list and is greater than the current cost, remove it from the closed list.
			if (itemToRemove != nullptr && cost < neighbor->GetgValue()) // replaced "Contains(neighbor) && ..." with this
			{
				inClosed = false;
				closedList.erase(closedList.begin() + index);
			}

			// If it's not on either list, update its cost and add it to the open list
			if (!openList->contains(neighbor) && (!inClosed))
			{
				neighbor->SetgValue(cost);
				openList->enqueue(neighbor);
				neighbor->SetParent(currentNode);
			}
		}
		return false;
	}
	else
	{
		ShortestPath();
		return true;
	}
}


void Graph::ShortestPath()
{
	currentNode = endNode;
	currentNode->isPath = true;

	while (currentNode != startNode)
	{
		currentNode = currentNode->GetParent();
		currentNode->isPath = true;
	}
}


// Sets up the start and end nodes, and calculates the H-Value of each node.
// Must be called before using any other part of the Graph class.
void Graph::SetStart(int x, int y)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->GetXCoord() == x && nodes[i]->GetYCoord() == y)
		{
			currentNode = nodes[i];
			currentNode->SetgValue(0);
			startNode = currentNode;
			startNode->SetgValue(0);
			openList->enqueue(startNode);
		}
	}
}

void Graph::SetEnd(int x, int y)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->GetXCoord() == x && nodes[i]->GetYCoord() == y)
		{
			endNode = (nodes[i]);
		}
	}

	// Calculate the H-Values of every node here.
	for (Node* n : nodes)
	{
		int dx = abs(n->GetXCoord() - endNode->GetXCoord());
		int dy = abs(n->GetYCoord() - endNode->GetYCoord());
		n->SethValue(dx + dy);
	}
}


// Getters
Node * Graph::GetCurrentNode()
{
	return currentNode;
}

Node * Graph::GetStartNode()
{
	return startNode;
}

Node * Graph::GetEndNode()
{
	return endNode;
}

vector<Node*> Graph::GetNodes()
{
	return nodes;
}

PriorityQueue * Graph::GetOpenList()
{
	return openList;
}
