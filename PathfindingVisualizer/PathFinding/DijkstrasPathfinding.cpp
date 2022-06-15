#include "DijkstrasPathfinding.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include "Vector2D.h"

DijkstrasPathfinding::DijkstrasPathfinding(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
}

//Find the shortest path using Dijkstras
//Make sure that the path is properly deleted when finished
//Set up very similar to A* execpt for the math of getting the path
Path DijkstrasPathfinding::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	//Make a queue for keeping track of Nodes cost (they are kepted in pairs)
	//List for already visited nodes
	std::priority_queue<std::pair<int, Node*>, std::vector<std::pair<int, Node*>>, std::greater<std::pair<int, Node*>>> nodesToVisit;
	std::list<Node*> visitedNodesList;
	nodesToVisit.push(std::make_pair(0, pFrom));
	visitedNodesList.push_back(pFrom);

#ifdef VISUALIZE_PATH
	mPath.clear();
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	Path path;
	Node* pCurrentNode = NULL;
	int currentCost = -1;
	bool toNodeAdded = false;
	while (pCurrentNode != pTo && nodesToVisit.size() > 0)
	{
		//Get the current node and cost, remove it for the list and add the node to the path
		//Create a Vector for the nodes connections
		pCurrentNode = nodesToVisit.top().second;
		currentCost = nodesToVisit.top().first;
		nodesToVisit.pop();
		path.addNode(pCurrentNode);
		std::vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		//add all connected nodes to the toVisit list
		//Makes sure to check if they are already there
		for (unsigned i = 0; i < connections.size(); ++i)
		{
			Connection* pConnection = connections[i];
			Node* pTempToNode = connections[i]->getToNode();
			if (!toNodeAdded &&
				!path.containsNode(pTempToNode) &&
				std::find(visitedNodesList.begin(), visitedNodesList.end(), pTempToNode) == visitedNodesList.end())
			{
				nodesToVisit.push(std::make_pair(currentCost + pConnection->getCost(), pTempToNode));
				visitedNodesList.push_back(pTempToNode);

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