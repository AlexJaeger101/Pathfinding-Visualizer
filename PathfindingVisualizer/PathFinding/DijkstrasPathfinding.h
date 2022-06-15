#pragma once
#include "GridPathfinder.h"
#include <vector>
#include <list>

class Path;
class Graph;
class GraphicsBuffer;
class Grid;

//Pathfinding class using the Dijkstras algorithm
class DijkstrasPathfinding : public GridPathfinder
{
public:
	//Constructor/Destructor
	DijkstrasPathfinding(Graph* pGraph);
	~DijkstrasPathfinding() {};

	//Find the shortest path using Dijkstras algorithm
	//Make sure that the path is properly deleted when finished
	Path findPath(Node* pFrom, Node* pTo);
};