#include "AStarPathfinding.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>

//Constructor
AStarPathfinding::AStarPathfinding(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
}

//Find the shortest path using A*, setup is very similar to Dijstars
//Help from Brendon on how to properly implement (specifically using the queue)
Path AStarPathfinding::findPath(Node* pFrom, Node* pTo)
{

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	//Create queue and list for keeping track of the nodes that where visited/have yet to visit
	//Add the from node to the lists
	std::priority_queue<std::pair<int, Node*>, std::vector<std::pair<int, Node*>>, std::greater<std::pair<int, Node*>>> toVisitQueue;
	std::list<Node*> visitedNodes;
	toVisitQueue.push(std::make_pair(0, pFrom));
	visitedNodes.push_back(pFrom);

#ifdef VISUALIZE_PATH
	mPath.clear();
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	Path path;
	Node* pCurrentNode = NULL;
	int currentPathCost = -1;
	bool toNodeAdded = false;
	while (pCurrentNode != pTo && toVisitQueue.size() > 0)
	{
		//Get the current node and cost, remove it for the queue and add the node to the path
		//Create a Vector for the nodes connections
		pCurrentNode = toVisitQueue.top().second;
		currentPathCost = toVisitQueue.top().first;
		toVisitQueue.pop();
		path.addNode(pCurrentNode);
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		//add all connected nodes to the toVisit list
		//Makes sure to check if they are already there
		for (unsigned i = 0; i < connections.size(); i++)
		{
			Connection* pConnection = connections[i];
			Node* pTempToNode = connections[i]->getToNode();

			if (!toNodeAdded &&
				!path.containsNode(pTempToNode) &&
				std::find(visitedNodes.begin(), visitedNodes.end(), pTempToNode) == visitedNodes.end())
			{
				toVisitQueue.push(std::make_pair(currentPathCost 
					+ pConnection->getCost() 
					+ Vector2D::getSquaredDistanceBetween(pTempToNode->getPos()
					, pTo->getPos()), pTempToNode));
				visitedNodes.push_back(pTempToNode);
				
				if (pTempToNode == pTo)
				{
					toNodeAdded = true;
				}

#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNode);
#endif

			}
		}
	}

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mPath = path;
#endif
	return path;

}