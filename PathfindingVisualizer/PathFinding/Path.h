#pragma once

#include <vector>
#include <Trackable.h>

class Node;

class Path:public Trackable
{
public:
	Path();
	~Path();

	Node* peekNode( int index ) const;
	Node* peekNextNode() const;
	Node* getAndRemoveNextNode();
	bool containsNode( Node* pNode );
	unsigned int getNumNodes() const { return mNodes.size(); };

	void addNode( Node* pNode );

	void clear();

private:
	std::vector<Node*> mNodes;

};