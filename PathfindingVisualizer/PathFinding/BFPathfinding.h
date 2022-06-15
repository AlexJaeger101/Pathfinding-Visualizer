#pragma once
#include "GridPathfinder.h"
#include <vector>
#include <list>

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//Pathfinding class using Breadth First Search
class BFPathfinding : public GridPathfinder
{
public:
	//Constructor/Destructor
	BFPathfinding(Graph* pGraph);
	~BFPathfinding() {};

	//Find the shortest path using Breadth First Search
	//Make sure that the path is properly deleted when finished
	Path findPath(Node* pFrom, Node* pTo);
};