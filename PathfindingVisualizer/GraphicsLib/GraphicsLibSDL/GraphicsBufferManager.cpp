#include <cassert>
#include "GraphicsBufferManager.h"
#include "GraphicsBuffer.h"
#include "GraphicsSystem.h"

using namespace std;

GraphicsBufferManager::GraphicsBufferManager(GraphicsSystem* pSystem)
	:mpGraphicsSystem(pSystem)
{
}

GraphicsBufferManager::~GraphicsBufferManager()
{
	clear();
}

void GraphicsBufferManager::init()
{
}

void GraphicsBufferManager::clear()
{
	map<GraphicsBufferID,GraphicsBuffer*>::iterator iter;
	for( iter = mBuffers.begin(); iter != mBuffers.end(); ++iter )
	{
		GraphicsBuffer* pBuff = iter->second;
		delete pBuff;
	}

	mBuffers.clear();
}

GraphicsBuffer* GraphicsBufferManager::loadBuffer( const GraphicsBufferID& id, const string& filename )
{
	GraphicsBuffer* pBuffer = NULL;
	
	//make sure this isn't already out there
	std::map<GraphicsBufferID, GraphicsBuffer*>::iterator iter = mBuffers.find( id );
	if( iter == mBuffers.end() )//not already in there
	{
		pBuffer = new GraphicsBuffer( mpGraphicsSystem->loadTexture(filename) );
		if( pBuffer->isValid() )
		{
			mBuffers[id] = pBuffer;
		}
		else
		{
			delete pBuffer;
			pBuffer = NULL;
		}
	}
	return pBuffer;
}

GraphicsBuffer* GraphicsBufferManager::manageBuffer(const GraphicsBufferID& id, GraphicsBuffer* pBuffer)
{
	//make sure this isn't already out there
	std::map<GraphicsBufferID, GraphicsBuffer*>::iterator iter = mBuffers.find(id);
	if (iter == mBuffers.end())//not already in there
	{
		if (pBuffer->isValid())
		{
			mBuffers[id] = pBuffer;
		}
	}
	else
	{
		assert(false);
	}

	return pBuffer;

}

GraphicsBuffer* GraphicsBufferManager::createGraphicsBuffer(const GraphicsBufferID& id, int width, int height, const Color& color)
{
	GraphicsBuffer* pBuffer = NULL;

	//make sure this isn't already out there
	std::map<GraphicsBufferID, GraphicsBuffer*>::iterator iter = mBuffers.find(id);
	if (iter == mBuffers.end())//not already in there
	{
		pBuffer = new GraphicsBuffer(mpGraphicsSystem->createTexture(width,height));
		if (pBuffer->isValid())
		{
			mBuffers[id] = pBuffer;
			mpGraphicsSystem->setBufferToColor(*pBuffer, color);
		}
		else
		{
			delete pBuffer;
			pBuffer = NULL;
		}
	}
	return pBuffer;

}

void GraphicsBufferManager::deleteBuffer(const GraphicsBufferID& id)
{
	//find the entry
	map<GraphicsBufferID, GraphicsBuffer*>::iterator iter = mBuffers.find(id);
	if (iter != mBuffers.end())//found!
	{
		GraphicsBuffer* pBuff = iter->second;
		delete pBuff;
		mBuffers.erase(iter);
	}
}

GraphicsBuffer* GraphicsBufferManager::getBuffer(const GraphicsBufferID& id)
{
	GraphicsBuffer* pReturn = NULL;

	map<GraphicsBufferID, GraphicsBuffer*>::iterator iter = mBuffers.find(id);
	if (iter != mBuffers.end())//found?
	{
		pReturn = iter->second;
	}

	return pReturn;
}

GraphicsBuffer* GraphicsBufferManager::createGridBuffer(const GraphicsBufferID& id, Uint32 bufferWidth, Uint32 bufferHeight, Uint32 cellWidth, Uint32 cellHeight, const Color& backgroundColor, const Color& gridColor, const Font& font)
{
	GraphicsBuffer* pBuffer = createGraphicsBuffer(id, bufferWidth, bufferHeight, backgroundColor);
	assert(pBuffer);

	//draw vertical lines
	for (Uint32 x = cellWidth; x < bufferWidth; x += cellWidth)
	{
		GraphicsSystem::renderLine(*pBuffer, Vector2D((int)x, 0), Vector2D((int)x, (int)bufferHeight), gridColor);
		GraphicsSystem::writeText(*pBuffer, Vector2D((int)x, 0), font, gridColor, to_string(x), Font::RIGHT);
	}
	//draw horizontal lines
	for (Uint32 y = cellHeight; y < bufferHeight; y += cellHeight)
	{
		GraphicsSystem::renderLine(*pBuffer, Vector2D(0, (int)y), Vector2D((int)bufferWidth, (int)y), gridColor);
		GraphicsSystem::writeText(*pBuffer, Vector2D(0, (int)y), font, gridColor, to_string(y), Font::LEFT);
	}
	return pBuffer;
}

