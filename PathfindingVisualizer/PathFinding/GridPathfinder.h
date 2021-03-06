#pragma once

#include "Pathfinder.h"
class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;

class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path findPath( Node* pFrom, Node* pTo ) = 0;
	
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
	Path getPath() const { return mPath; };
	double getElapsedTime() const { return mTimeElapsed; };
	int getNumVisitedNodes() const { return mVisitedNodes.size(); };
protected:
	std::vector<Node*> mVisitedNodes;
	Path mPath;
	GridVisualizer* mpVisualizer;
#endif

	double mTimeElapsed;
};