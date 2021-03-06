#include "GridGraph.h"
#include "Connection.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include <vector>

GridGraph::GridGraph( Grid* pGrid )
:Graph()
,mpGrid(pGrid)
{
}

GridGraph::~GridGraph()
{
}

void GridGraph::init()
{
	static float sqrt2 = sqrt(2.0f);

	if( mpGrid != NULL )
	{
		int numGridIndices = mpGrid->getGridWidth() * mpGrid->getGridHeight();

		//size the node vector
		mNodes.resize( numGridIndices, NULL );
		
		//go through all elements of the grid and make Nodes for them - also add them to Node vector
		for( int i=0; i<numGridIndices; i++ )
		{
			float cost = 1.0f;
			if (i < 100)
			{
				cost = 99.0f;
			}

			//create a Node for this location
			Node* pNode = new Node( i , Vector2D(i % mpGrid->getGridWidth(), i / mpGrid->getGridWidth()), cost);
			//add to NodeVector
			mNodes[i] = pNode;
		}

		//go through all elements of the grid - get adjacencies and create connections for each
		for( int i=0; i<numGridIndices; i++ )
		{
			//get the Node for this location
			Node* pFromNode = mNodes[i];

			//get all the adjacencies
			std::vector<int> adjacencies = mpGrid->getAdjacentIndices( i );

			//create a vector of Connections
			std::vector<Connection*> connections;

			//go through all adjacent indices, get the nodes for them, create connections, and add to connections map
			for( unsigned int adjIndex = 0; adjIndex < adjacencies.size(); adjIndex++ )
			{
				//check for blocking terrain
				if( mpGrid->getValueAtIndex(adjacencies[adjIndex]) != BLOCKING_VALUE )
				{
					Node* pToNode = mNodes[ adjacencies[adjIndex] ];//find to node

					float cost = 1.0f;
					if (mpGrid->areCellsDiagonal(pFromNode->getId(), pToNode->getId()))
						cost = sqrt2;
					Connection* pConnection = new Connection( pFromNode, pToNode, cost);//create a connection

					//add connection to connection vectors
					mConnections.push_back( pConnection );
					connections.push_back( pConnection );
				}
			}

			//put temp connection vector into map
			mConnectionMap[i] = connections;
		}
	}
}
