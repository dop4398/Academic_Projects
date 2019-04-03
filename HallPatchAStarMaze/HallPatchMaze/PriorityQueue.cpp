#include "stdafx.h"
#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()
{
	// Create queue and thats it
	queue = new vector<Node*>();
}


PriorityQueue::~PriorityQueue()
{
	delete queue;
	delete temp;
}


// Just a helper to check if the queue is empty
bool PriorityQueue::isEmpty()
{
	return ((*queue).empty());
}


// Returns top node
Node* PriorityQueue::peek()
{
	return (*queue)[0];
}



// Take node out of queue, uses helper	
Node* PriorityQueue::dequeue()
{
	Node* returnNode = (*queue)[0];

	(*queue)[0] = (*queue)[(*queue).size() - 1];
	(*queue).pop_back();

	if (!isEmpty())
	{
		downheap(0);
	}

	int rx = returnNode->GetXCoord();
	int ry = returnNode->GetYCoord();

	return returnNode;
}


// Add to the priority queue, uses helper
void PriorityQueue::enqueue(Node* newNode)
{
	(*queue).push_back(newNode);
	upheap((*queue).size() - 1);
}




// Sizable helper method for enqueue
void PriorityQueue::upheap(int index)
{
	int parentPos = ((index - 1) / 2);
	if (index > 0 && (*((*queue)[parentPos])).GetfValue() > (*((*queue)[index])).GetfValue())
	{
		temp = ((*queue)[index]);
		(*queue)[index] = (*queue)[parentPos];
		(*queue)[parentPos] = temp;
		upheap(parentPos);
	}
}




// Sizable helper method for dequeue
void PriorityQueue::downheap(int index)
{

	Node* original = (*queue)[index];

	// If left node exists
	if (index * 2 + 1 < (*queue).size())
	{

		Node* left = (*queue)[index * 2 + 1];


		// If both left and right nodes exist
		if (index * 2 + 2 < (*queue).size())
		{

			Node* right = (*queue)[index * 2 + 2];

			// If left node is smaller than the right one
			if ((*left).GetfValue() < (*right).GetfValue())
			{
				// And smaller than the current node, then swap them
				if ((*left).GetfValue() < (*original).GetfValue())
				{
					(*queue)[index * 2 + 1] = original;
					(*queue)[index] = left;
					downheap(index * 2 + 1);
				}
			}
			// If right node is smaller than the left one
			else
			{
				// And smaller than the current node, then swap them
				if ((*right).GetfValue() < (*original).GetfValue())
				{
					(*queue)[index * 2 + 2] = original;
					(*queue)[index] = right;
					downheap(index * 2 + 2);
				}
			}
		}
		else
		{
			// If left is smaller than the current node, then swap them
			if ((*left).GetfValue() < (*original).GetfValue())
			{
				(*queue)[index * 2 + 1] = original;
				(*queue)[index] = left;
				downheap(index * 2 + 1);
			}
		}

	}

	// If right node exists
	else if (index * 2 + 2 < (*queue).size())
	{
		Node* right = (*queue)[index * 2 + 2];
		// And right is smaller than the current node, then swap them
		if ((*right).GetfValue() < (*original).GetfValue())
		{
			(*queue)[index * 2 + 2] = original;
			(*queue)[index] = right;
			downheap(index * 2 + 2);
		}
	}
}

// Checks if the entered node is in the queue.
bool PriorityQueue::contains(Node * node)
{
	// For loop that finds node
	for (Node* n : *queue)
	{
		if (node->GetXCoord() == n->GetXCoord() && node->GetYCoord() == n->GetYCoord())
		{
			return true;
		}
	}
	return false;
}

void PriorityQueue::remove(Node * node)
{
	// For loop that finds the node and down heaps from there
	for (int i = 0; i < (*queue).size(); i++)
	{
		if (node->GetXCoord() == ((*queue)[i])->GetXCoord() && node->GetYCoord() == ((*queue)[i])->GetYCoord())
		{
			Node* returnNode = (*queue)[i];

			(*queue)[0] = (*queue)[(*queue).size() - 1];
			(*queue).pop_back();

			if (!isEmpty())
			{
				downheap(i);
				
			}
		}
	}
}
