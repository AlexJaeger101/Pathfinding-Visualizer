#pragma once
#include "GridPathfinder.h"
#include <vector>
#include <list>

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//Pathfinding class using A*
class AStarPathfinding : public GridPathfinder
{
public:
	//Constructor/Destructor
	AStarPathfinding(Graph* pGraph);
	~AStarPathfinding() {};

	//Find the shortest path using A*
	//Make sure that the path is properly deleted when finished
	Path findPath(Node* pFrom, Node* pTo);
};