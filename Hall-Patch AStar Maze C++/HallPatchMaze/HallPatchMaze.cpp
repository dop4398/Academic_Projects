// HallPatchMaze.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "Graph.h"

//Fields for the map
int** map;
int wid;
int leng;

//Temp field
int num = 0;
char names[] = "Adam Hall, David Patch";

//Start and end coordinates
int startX;
int startY;

int endX;
int endY;

// Graph object
Graph* graph;

// Information for next position
bool firstNode = true;
Node* currentPathNode;

__declspec(dllexport) char* GetTeam()
{
	return names;
}

__declspec(dllexport) bool SetMaze(const int** data, int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		return false;
	}

	//Set empty 2d array	
	wid = width;
	leng = height;
	map = new int*[wid];
	for (int i = 0; i < wid; i++)
	{
		map[i] = new int[leng];
	}

	//Sets the data in it
	for (int i = 0; i < wid; i++)
	{
		for (int j = 0; j < leng; j++)
		{
			map[i][j] = data[i][j];
		}
	}
	
	// Create the Graph object
	graph = new Graph(wid, leng, map);

	return true;
}

__declspec(dllexport) int** GetMaze(int& width, int& height)
{
	// Try catch and if(map) both to make sure that map is valid
	// Since I am not sure how an uninitialized variable will interact in a DLL
	try
	{
		if (map)
		{
			//Sets pointer width and height
			width = wid;
			height = leng;

			//Return 2d array
			return (int**)map;
		}

		return nullptr;

	}
	catch (const std::exception&)
	{
		return nullptr;
	}
	
}

__declspec(dllexport) bool GetNextPosition(int& xpos, int& ypos)
{
	// Runs pathfinding when first asked
	if (firstNode)
	{
		currentPathNode = graph->GetStartNode();
		currentPathNode->printed = true;
		currentPathNode->isPath = true;
		bool aStarDone = false;
		while(!aStarDone)
		{
			if (graph->GetOpenList()->isEmpty())
			{
				cout << "Hit a wall!" << endl;
				graph->SetStart(graph->GetEndNode()->GetXCoord(), graph->GetEndNode()->GetYCoord());
				return false;
			}
			cout << graph->GetCurrentNode()->GetfValue() << ":  " << graph->GetCurrentNode()->GetXCoord() << " , " << graph->GetCurrentNode()->GetYCoord() << endl;
			aStarDone = graph->AStarAlgorithm();
		}
		firstNode = false;
	}

	// Then returns locations based on pathfinding that was done
	xpos = currentPathNode->GetXCoord();
	ypos = currentPathNode->GetYCoord();

	if (currentPathNode->GetXCoord() == graph->GetEndNode()->GetXCoord() && currentPathNode->GetYCoord() == graph->GetEndNode()->GetYCoord())
	{
		delete currentPathNode;
		for (int i = 0; i < wid; i++)
		{
			delete map[i];
		}
		delete map;
		 // Pointers deleted here (except for graph)
		if (_CrtDumpMemoryLeaks())
		{
			// The memory leak is from the Graph pointer in HallPatchMaze.cpp not getting deleted.
			// Deleting the pointer in MazeDestructor() above results in the player only making it to the space right before the goal.
			// If "delete graph" is added to the method, then this message is never printed, as there are no other memory leaks.
			cout << "Memory Leaks found!" << endl;		
		}
		return false;
	}

	
	for (Node* neighbor : currentPathNode->GetNeightbors())
	{
		if (neighbor->isPath && !neighbor->printed)
		{
			currentPathNode->printed = true;
			currentPathNode = neighbor;
		}
	}

	return true;
}
	


//Get and set methods for start and end points
__declspec(dllexport) bool SetStart(int xpos, int ypos)
{
	if (xpos >= 0 && xpos < wid && ypos >= 0 && ypos < leng)
	{
		startX = xpos;
		startY = ypos;
		graph->SetStart(startX, startY);
		return true;
	}

	return false;
}

__declspec(dllexport) bool GetStart(int& xpos, int& ypos)
{
	try
	{
		if (startX && startY)
		{
			xpos = startX;
			ypos = startY;

			return true;
		}
		return false;
	}
	catch (const std::exception&)
	{
		return false;
	}
	
}

__declspec(dllexport) bool SetEnd(int xpos, int ypos)
{
	if(xpos >= 0 && ypos >= 0)
	{
		endX = xpos;
		endY = ypos;
		graph->SetEnd(endX, endY);

		return true;
	}

	return false;
}

__declspec(dllexport) bool GetEnd(int& xpos, int& ypos)
{
	// Try catch and if(variables) both to make sure that variables are valid
	// Since I am not sure how an uninitialized variable will interact in a DLL
	try
	{
		if (endX && endY)
		{
			xpos = endX;
			ypos = endY;

			return true;
		}

		return false;

	}
	catch (const std::exception&)
	{
		return false;
	}
}


__declspec(dllexport) bool Restart()
{
	try
	{
		for (Node* n : graph->GetNodes())
		{
			n->Reset();
		}
		graph->SetStart(startX, startY);

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}


// Deletes the pointers created in this cpp
__declspec(dllexport) bool MazeDestructor()
{
	try
	{
		delete currentPathNode;
		for (int i = 0; i < wid; i++)
		{
			delete map[i];
		}
		delete map;
		//delete graph;

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}