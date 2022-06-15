#pragma once

#include <Trackable.h>
#include "Defines.h"
#include <unordered_map>

class Sprite;
class GraphicsBuffer;

class SpriteManager: public Trackable
{
public:
	SpriteManager();
	~SpriteManager();

	Sprite* createAndManageSprite( const IDType& id, GraphicsBuffer* pBuffer, float sx, float sy, float sw, float sh );
	Sprite* getSprite( const IDType& id );
	void destroySprite( const IDType& id );

private:
	std::unordered_map<IDType,Sprite*> mpSprites;
};