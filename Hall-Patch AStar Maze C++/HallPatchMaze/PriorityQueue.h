#pragma once
#include "Node.h"

class PriorityQueue
{

private:
	vector<Node*> *queue;
	Node* temp;
public:
	PriorityQueue();
	~PriorityQueue();
	bool isEmpty();
	Node* peek();
	Node* dequeue();
	void enqueue(Node* newNode);
	void upheap(int index);
	void downheap(int index);
	bool contains(Node* node);
	void remove(Node* node);
};

