#include "GraphicsBuffer.h"
#include "GraphicsSystem.h"
#include "Renderer.h"
#include "Color.h"
#include <cassert>

int GraphicsBuffer::getWidth() const
{
	if (mValid)
	{
		int width;
		int retVal = SDL_QueryTexture(mpTexture, NULL, NULL, &width, NULL);
		assert(retVal == 0);
		if (retVal == 0)
		{
			return width;
		}
	}
	else if (mpTexture == NULL)//default rendering texture
	{
		return (int)GraphicsSystem::getDisplayWidth();
	}
	return INVALID_DIMENSION;
}

int GraphicsBuffer::getHeight() const
{
	if (mValid)
	{
		int height;
		int retVal = SDL_QueryTexture(mpTexture, NULL, NULL, NULL, &height);
		assert(retVal == 0);
		if (retVal == 0)
		{
			return height;
		}
	}
	else if (mpTexture == NULL)//default rendering texture
	{
		return (int)GraphicsSystem::getDisplayHeight();
	}
	return INVALID_DIMENSION;
}

Vector2D GraphicsBuffer::getSize() const
{
	int width, height;
	if (mValid)
	{
		int retVal = SDL_QueryTexture(mpTexture, NULL, NULL, &width, &height);
		assert(retVal == 0);
		if (retVal == 0)
		{
			return Vector2D(width,height);
		}
	}
	else if (mpTexture == NULL)//default rendering texture
	{
		width = (int)GraphicsSystem::getDisplayWidth();
		height = (int)GraphicsSystem::getDisplayHeight();
		return Vector2D(width, height);
	}

	return ZERO_VECTOR2D;
}

void GraphicsBuffer::getDimensions(int& width, int& height) const
{
	if (mValid)
	{
		int retVal = SDL_QueryTexture(mpTexture, NULL, NULL, &width, &height);
		assert(retVal == 0);
		if (retVal == 0)
		{
			return;
		}
	}
	else if (mpTexture == NULL)//default rendering texture
	{
		width = (int)GraphicsSystem::getDisplayWidth();
		height = (int)GraphicsSystem::getDisplayHeight();
		return;
	}

	width = INVALID_DIMENSION;
	height = INVALID_DIMENSION;
}

//used to create the back buffer
GraphicsBuffer::GraphicsBuffer(const Renderer* pRenderer)
{
	mpTexture = SDL_GetRenderTarget(pRenderer->mpRenderer);
	if (mpTexture != NULL)
	{
		mValid = true;
	}

}

GraphicsBuffer::GraphicsBuffer(SDL_Texture* pTexture)
	:mpTexture(pTexture)
{
	if (pTexture != NULL)
	{
		mValid = true;
	}
}

GraphicsBuffer::GraphicsBuffer(Renderer* pRenderer, unsigned int width, unsigned int height, const Color& color)
{
	mpTexture = SDL_CreateTexture(pRenderer->mpRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	GraphicsSystem::setBufferToColor(*this,color);
}

GraphicsBuffer::~GraphicsBuffer()
{
	if (mValid)
	{
		SDL_DestroyTexture(mpTexture);
	}
}

GraphicsBuffer * GraphicsBuffer::clone(Renderer* pRenderer) const
{
	GraphicsBuffer* pNewBuffer = new GraphicsBuffer(pRenderer,getWidth(), getHeight());
	GraphicsSystem::draw(*pNewBuffer, ZERO_VECTOR2D, *this);
	return pNewBuffer;
}

