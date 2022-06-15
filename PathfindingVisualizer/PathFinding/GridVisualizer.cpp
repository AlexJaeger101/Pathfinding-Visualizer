#include "GridVisualizer.h"
#include <GraphicsSystem.h>
#include <GraphicsBuffer.h>
#include "Grid.h"
#include "Game.h"
#include <Vector2D.h>

GridVisualizer::GridVisualizer( Grid* pGrid )
:mpGrid(pGrid)
,mDirty(true)
{
}

GridVisualizer::~GridVisualizer()
{
}

void GridVisualizer::refresh()
{
	const Color& color = BLACK_COLOR; 
	if( mDirty )
	{
		//remove old entries first
		removeAllEntriesOfColor( color );

		int size = mpGrid->getGridWidth() * mpGrid->getGridHeight();
		//get any non-zero squares and send them to the visualizer
		for( int i=0; i<size; i++ )
		{
			if( mpGrid->getValueAtIndex(i) != 0 )
			{
				addColor( i, color );
			}
		}
	}
	mDirty = false;
}

void GridVisualizer::addColor( int index, const Color& color )
{
	//set dirty flag
	mDirty = true;

	//find the existing entry in the map (if it exists)
	auto iter = mColormap.find( color );

	if( iter != mColormap.end() )//found the existing entry
	{
		iter->second.push_back( index );
	}
	else
	{
		//create a vector to put in the map
		std::vector<int> colorVector;
		colorVector.push_back( index );

		//put the vector into the map
		mColormap[color] = colorVector;
	}
}

void GridVisualizer::removeAllEntriesOfColor( const Color& color )
{
	//find the existing entry in the map (if it exists)
	auto iter = mColormap.find( color );
	
	if( iter != mColormap.end() )
	{
		mColormap.erase( iter );
	}
}

void GridVisualizer::draw( GraphicsBuffer& dest )
{
	if( mDirty )
	{
		refresh();
	}

	int gridHeight = mpGrid->getGridHeight();
	int gridWidth = mpGrid->getGridWidth();
	int numSquares = gridWidth * gridHeight;
	Vector2D cellSize = mpGrid->getCellSize();
	int xSize = (int)cellSize.getX();
	int ySize = (int)cellSize.getY();

	GraphicsSystem* pGraphicsSystem = gpGame->getGraphicsSystem();

	for( auto iter = mColormap.begin(); iter != mColormap.end(); ++iter )
	{
		std::vector<int> theIndices = iter->second;

		for( unsigned int i=0; i<theIndices.size(); i++ )
		{
			Vector2D ulPos = mpGrid->getULCornerOfCell( theIndices[i] );
			GraphicsSystem::renderRect(*pGraphicsSystem->getBackBuffer(), ulPos, xSize, ySize, iter->first);
		}
	}
}