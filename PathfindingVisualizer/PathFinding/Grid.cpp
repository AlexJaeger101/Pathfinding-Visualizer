#include "Grid.h"
#include "GraphicsSystem.h"
#include "Vector2D.h"
#include <memory.h>
#include <cassert>
#include <cmath>

Grid::Grid( int pixelWidth, int pixelHeight, int cellSizeX, int cellSizeY )
:mPixelWidth(pixelWidth)
,mPixelHeight(pixelHeight)
, mCellSizeX(cellSizeX)
, mCellSizeY(cellSizeY)
{
	mGridWidth = pixelWidth / cellSizeX;
	mGridHeight = pixelHeight / cellSizeY;
	mpArray = new MultiDimensionalArray<int>(mGridHeight, mGridWidth,0);
}

Grid::~Grid()
{
	delete mpArray;
}

int Grid::getCellIndexFromPixelXY( int x, int y ) const
{
	x /= mCellSizeX;
	y /= mCellSizeY;
	return mpArray->convertCoordsToIndex(x, y);
}

int Grid::getValueAtIndex( int index ) const
{
	return *(mpArray->get(index));
}

int Grid::getValueAtPixelXY( int x, int y ) const
{
	int index = getCellIndexFromPixelXY( x, y );
	return getValueAtIndex( index );
}

Vector2D Grid::getCellCoordsFromIndex(int index) const
{
	unsigned int x, y;
	mpArray->convertIndexToCoords(index, x, y);
	return Vector2D((int)x,(int)y);
}
	
void Grid::setValueAtIndex( int index, int val)
{
	int* pStoredVal = mpArray->get(index);
	assert(pStoredVal);
	if (pStoredVal)
		* pStoredVal = val;
}

void Grid::setValueAtPixelXY( int x, int y, int val)
{
	int index = getCellIndexFromPixelXY( x, y );
	setValueAtIndex( index, val );
}

Vector2D Grid::getULCornerOfCell( int index ) const
{
	//int squareY = index / mGridWidth;
	//int squareX = index % mGridWidth;
	unsigned int x, y;
	mpArray->convertIndexToCoords(index, x, y);
	Vector2D pos( (float)(x * mCellSizeX), (float)(y * mCellSizeY) );
	return pos;
}

bool Grid::areCellsDiagonal(int index1, int index2)
{
	unsigned int x1, y1;
	mpArray->convertIndexToCoords(index1, x1, y1);
	unsigned int x2, y2;
	mpArray->convertIndexToCoords(index2, x2, y2);

	unsigned int sum = abs((int)(x1 - x2)) + abs((int)(y1 - y2));
	if(  sum == 2)
		return true;
	else
		return false;
}

//get adjacent grid square indices
std::vector<int> Grid::getAdjacentIndices( int theSquareIndex ) const
{
	std::vector<int> indices;

	static const int NUM_DIRS = 8;
	//						        N  NE	E  SE	S	SW	 W	NW
	static int xMods[NUM_DIRS] = {	0,	1,	1,	1,	0,	-1,	-1,	-1 };
	static int yMods[NUM_DIRS] = { -1, -1,	0,	1,	1,	 1,	 0,	-1 };

	//find the x,y of the passed in index
	//int x = theSquareIndex % mGridWidth;
	//int y = theSquareIndex / mGridWidth;
	unsigned int x, y;
	mpArray->convertIndexToCoords(theSquareIndex, x, y);

	for( int i=0; i<NUM_DIRS; i++ )
	{
		//calc adjacent x,y
		int adjX = x + xMods[i];
		int adjY = y + yMods[i];

		//convert back to an index if on the Grid
		if( adjX >= 0 && adjX < mGridWidth && adjY >=0 && adjY < mGridHeight )
		{
			int adjIndex = ( adjY * mGridWidth ) + adjX;

			//add to vector of indices
			indices.push_back( adjIndex );
		}
	}
	return indices;
}


void Grid::save( std::ofstream& file )
{
	int numSquares = mGridWidth * mGridHeight;
	for( int i=0; i<numSquares; i++ )
	{
		file << *mpArray->get(i) << " ";
	}
}

void Grid::load( std::ifstream& file )
{
	int numSquares = mGridWidth * mGridHeight;
	for( int i=0; i<numSquares; i++ )
	{
		int* pStoredVal = mpArray->get(i);
		int val;
		file >> val;
		if (pStoredVal)
			*pStoredVal = val;
	}
}
