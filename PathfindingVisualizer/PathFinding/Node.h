#pragma once

#include <Trackable.h>
#include "Vector2D.h"

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node( const NODE_ID& id, const Vector2D pos, const int cost = 1 );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};
	const Vector2D getPos() const { return mPos; };
	const int getCost() const { return mCost; };

private:
	const NODE_ID mId;
	const Vector2D mPos;
	const int mCost;
};