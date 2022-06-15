#pragma once

#include <fstream>
#include <vector>
#include <Trackable.h>
#include <MultiDimensionalArray.h>
#include <Vector2D.h>

const int CLEAR_VALUE = 0;
const int BLOCKING_VALUE = 1;

class Grid:public Trackable
{
public:
	Grid( int pixelWidth, int pixelHeight, int cellSizeX, int cellSizeY );
	~Grid();

	int getCellIndexFromPixelXY( int x, int y ) const;

	int getValueAtIndex( int index ) const;
	int getValueAtPixelXY( int x, int y ) const;
	Vector2D getCellCoordsFromIndex(int index) const;
	void setValueAtIndex( int index, int val );
	void setValueAtPixelXY( int x, int y, int val );
	Vector2D getULCornerOfCell( int index ) const;

	//accessors
	int getPixelWidth() const { return mPixelWidth; };
	int getPixelHeight() const { return mPixelHeight; };
	int getGridWidth() const { return mGridWidth; };
	int getGridHeight() const { return mGridHeight; };
	Vector2D getCellSize() const { return Vector2D(mCellSizeX,mCellSizeY); };
	bool areCellsDiagonal(int index1, int index2);

	//get adjacent grid square indices
	std::vector<int> getAdjacentIndices( int theSquareIndex ) const;

	void save( std::ofstream& file );
	void load( std::ifstream& file );

private:
	MultiDimensionalArray<int>* mpArray;
	int mPixelWidth;
	int mPixelHeight;
	int mCellSizeX;
	int mCellSizeY;
	int mGridWidth;
	int mGridHeight;
};