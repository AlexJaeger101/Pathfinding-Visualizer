#include "GridPathfinder.h"
#include "GridGraph.h"
#include "GridVisualizer.h"
#include "Path.h"
#include "Game.h"
#include "GraphicsBuffer.h"
#include <cassert>

GridPathfinder::GridPathfinder( GridGraph* pGraph )
:Pathfinder(pGraph)
,mTimeElapsed(0.0)
{
#ifdef VISUALIZE_PATH
	mpVisualizer = NULL;
#endif
}

GridPathfinder::~GridPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpVisualizer;
#endif
}
#ifdef VISUALIZE_PATH
void GridPathfinder::drawVisualization( Grid* pGrid, GraphicsBuffer* pDest )
{
	delete mpVisualizer;
	mpVisualizer = new GridVisualizer( pGrid );
	static Color pathColor( 0, 129, 128 );
	static Color visitedColor( 0, 128, 0 );
	static Color startColor = Color(1, 255, 128);
	static Color stopColor = Color(1, 128, 255);

	unsigned int numNodes = mPath.getNumNodes();
	if(numNodes > 0 )
	{
		Color currentPathColor = pathColor;

		for( unsigned int i=0; i< numNodes; i++ )
		{
			mpVisualizer->addColor( mPath.peekNode(i)->getId(), currentPathColor );
			currentPathColor = GraphicsSystem::lerpColor(GREEN_COLOR, RED_COLOR, (double)i / numNodes);
		}

		mpVisualizer->addColor( mPath.peekNode(0)->getId(), startColor );
		mpVisualizer->addColor( mPath.peekNode(numNodes-1)->getId(), stopColor );
	}

	mpVisualizer->draw(*pDest);
}
#endif