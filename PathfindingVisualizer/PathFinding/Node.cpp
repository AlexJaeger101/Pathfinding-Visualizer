#include "Node.h"

Node::Node()
:mId(BAD_NODE_ID)
, mPos(ZERO_VECTOR2D)
, mCost(-1)
{
}

Node::Node( const NODE_ID& id )
:mId(id)
, mPos(ZERO_VECTOR2D)
, mCost(-1)
{
}

Node::Node(const NODE_ID& id, const Vector2D pos, const int cost)
	:mId(id)
	, mPos(pos)
	, mCost(cost)
{
}

Node::~Node()
{
}