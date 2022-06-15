#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Color.h"
#include "Renderer.h"
#include "Font.h"
#include "SDL_FontCache.h"
#include "SDL2_gfxPrimitives.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cassert>

using namespace std;

Renderer* GraphicsSystem::mpRenderer = NULL;
GraphicsBuffer* GraphicsSystem::mpBackBuffer=NULL;
unsigned int GraphicsSystem::mWidth = 0;
unsigned int GraphicsSystem::mHeight = 0;


GraphicsSystem::GraphicsSystem()
{
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

bool GraphicsSystem::init(unsigned int width, unsigned int height)
{
	//possible enhancement - if width and height is different than current initted display - cleanup and re-init with new size
	if (!mIsInitted)
	{
		//create window
		mpWindow = SDL_CreateWindow("TEST APP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		if (mpWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}

		mpRenderer = new Renderer(mpWindow);
		mpBackBuffer = mpRenderer->makeBackBuffer();

		//enable PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		//enable TTF support
		if (TTF_Init() < 0)
		{
			printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
			return false;
		}

		mWidth = width;
		mHeight = height;

		mIsInitted = true;
	}
	return true;
}

void GraphicsSystem::cleanup()
{
	if (mIsInitted)
	{
		delete mpBackBuffer;
		mpBackBuffer = NULL;

		delete mpRenderer;
		mpRenderer = NULL;

		SDL_DestroyWindow(mpWindow);
		mpWindow = NULL;

		//Quit SDL subsystems
		IMG_Quit();
		TTF_Quit();
		mWidth = 0;
		mHeight = 0;
		mIsInitted = false;
	}
}

void GraphicsSystem::flip()
{
	assert(mIsInitted);

	//present renderer
	SDL_RenderPresent(mpRenderer->mpRenderer);
}

void GraphicsSystem::draw(const Vector2D& destLoc, const GraphicsBuffer& src, double rotationInRadians /*= 0.0*/, double scale /* = 1.0*/, double alpha /*= 1.0*/, const Color& color /*= BLACK_COLOR*/)
{
	draw(*getBackBuffer(), destLoc, src, rotationInRadians, scale, alpha, color);
}

void GraphicsSystem::draw(GraphicsBuffer& dest, const Vector2D& loc, const GraphicsBuffer& src, double rotationInRadians /* = 0.0*/, double scale /* = 1.0*/, double alpha /*= 1.0*/, const Color& color /*= BLACK_COLOR*/)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Texture* pSrcTexture = src.mpTexture;
	Uint8 oldR, oldG, oldB;
	SDL_GetTextureColorMod(pSrcTexture, &oldR, &oldG, &oldB);
	Uint8 oldAlpha;
	SDL_GetTextureAlphaMod(pSrcTexture, &oldAlpha);

	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	Vector2D srcSize = src.getSize();
	Vector2D srcLoc = ZERO_VECTOR2D;
	SDL_Rect srcRect;
	srcRect.w = (int)srcSize.getX();
	srcRect.h = (int)srcSize.getY();
	srcRect.x = (int)srcLoc.getX();
	srcRect.y = (int)srcLoc.getY();
	SDL_Rect destRect;
	destRect.w = (int)(srcRect.w*scale);
	destRect.h = (int)(srcRect.h*scale);
	destRect.x = (int)loc.getX();
	destRect.y = (int)loc.getY();
	double rotationInDegrees = rotationInRadians * (180.0 / M_PI);

	SDL_SetRenderTarget(pRenderer, dest.mpTexture);
	SDL_SetTextureColorMod(pSrcTexture, color.getR(), color.getG(), color.getB());
	SDL_SetTextureAlphaMod(pSrcTexture, getIntAlphaValue(alpha));

	SDL_RenderCopyEx(pRenderer, pSrcTexture, &srcRect, &destRect, rotationInDegrees, NULL, SDL_FLIP_NONE);

	SDL_SetTextureColorMod(pSrcTexture, oldR, oldG, oldB);
	SDL_SetTextureAlphaMod(pSrcTexture, oldAlpha);
	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::draw(const Vector2D& destLoc, const Sprite& sprite, double rotationInRadians /*= 0.0*/, double scale /* = 1.0*/, double alpha /*= 1.0*/, const Color& color /*= BLACK_COLOR*/)
{
	draw(*getBackBuffer(), destLoc, sprite, rotationInRadians, scale, alpha, color);
}

void GraphicsSystem::draw(GraphicsBuffer& dest, const Vector2D& loc, const Sprite& sprite, double rotationInRadians /* = 0.0*/, double scale /* = 1.0*/, double alpha /*= 1.0*/, const Color& color /*= BLACK_COLOR*/)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Texture* pSrcTexture = sprite.getBuffer()->mpTexture;
	Uint8 oldR, oldG, oldB;
	SDL_GetTextureColorMod(pSrcTexture, &oldR, &oldG, &oldB);
	Uint8 oldAlpha;
	SDL_GetTextureAlphaMod(pSrcTexture, &oldAlpha);

	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	Vector2D srcSize(sprite.getWidth(),sprite.getHeight());
	Vector2D srcLoc = sprite.getSourceLoc();
	SDL_Rect srcRect;
	srcRect.w = (int)srcSize.getX();
	srcRect.h = (int)srcSize.getY();
	srcRect.x = (int)srcLoc.getX();
	srcRect.y = (int)srcLoc.getY();
	SDL_Rect destRect;
	destRect.w = (int)(srcRect.w*scale);
	destRect.h = (int)(srcRect.h*scale);
	destRect.x = (int)loc.getX();
	destRect.y = (int)loc.getY();
	double rotationInDegrees = rotationInRadians * (180.0 / M_PI);
	SDL_SetRenderTarget(pRenderer, dest.mpTexture);
	SDL_SetTextureColorMod(pSrcTexture, color.getR(), color.getG(), color.getB());
	SDL_SetTextureAlphaMod(pSrcTexture,getIntAlphaValue(alpha));
	SDL_RenderCopyEx(pRenderer, pSrcTexture, &srcRect, &destRect, rotationInDegrees, NULL, SDL_FLIP_NONE);

	SDL_SetTextureColorMod(pSrcTexture, oldR, oldG, oldB);
	SDL_SetTextureAlphaMod(pSrcTexture, oldAlpha);
	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::writeText(const Vector2D& destLoc, const Font& font, const Color& color, const std::string& text, Font::Alignment align /*= Font::LEFT*/)
{
	//FC_Draw(font.getFont(), mpRenderer->mpRenderer, destLoc.getX(), destLoc.getY(), text.c_str());
	FC_Effect effect;
	effect.alignment = (FC_AlignEnum)align;
	effect.color = getSDLColorFromColor(color);
	effect.scale = FC_MakeScale(1.0f, 1.0f);

	FC_DrawEffect(font.getFont(), mpRenderer->mpRenderer, destLoc.getX(), destLoc.getY(), effect, text.c_str());
};

void GraphicsSystem::writeText(GraphicsBuffer& dest, const Vector2D& destLoc, const Font& font, const Color& color, const std::string& text, Font::Alignment align /*= Font::LEFT*/)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	writeText(destLoc, font, color, text, align);
	restoreRenderTarget(pOldTarget);

}

void GraphicsSystem::setBufferToColor(GraphicsBuffer& buffer, const Color& color)
{
	SDL_Texture* pOldTarget = setRenderTarget(buffer);
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(pRenderer, &r, &g, &b, &a);

	SDL_SetTextureBlendMode(buffer.mpTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_RenderClear(pRenderer);

	SDL_SetRenderDrawColor(pRenderer, r, g, b, a);

	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::saveBufferToFile(const GraphicsBuffer& buffer, const std::string& filename)
{
	//based on code found in this thread: https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file

	SDL_Texture* target = SDL_GetRenderTarget(mpRenderer->mpRenderer);
	SDL_SetRenderTarget(mpRenderer->mpRenderer, buffer.mpTexture);
	int width, height;
	buffer.getDimensions(width, height);

	if (width == INVALID_DIMENSION || height == INVALID_DIMENSION)
	{
		width = mWidth;
		height = mHeight;
	}

	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(mpRenderer->mpRenderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, filename.c_str());
	SDL_FreeSurface(surface);
	SDL_SetRenderTarget(mpRenderer->mpRenderer, target);
}

//deprecated!!!
void GraphicsSystem::renderFilledRect(GraphicsBuffer& dest, const Vector2D& destLoc, int width, int height, const Color& color)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;

	//get old render color
	unsigned char oldR, oldG, oldB, oldA;
	SDL_GetRenderDrawColor(pRenderer, &oldR, &oldG, &oldB, &oldA);

	SDL_SetRenderDrawColor(pRenderer, color.getR(), color.getG(), color.getB(), color.getA());
	SDL_Rect rect = { (int)destLoc.getX(), (int)destLoc.getY(), width, height };
	int retVal = SDL_RenderFillRect(pRenderer, &rect);

	SDL_SetRenderDrawColor(pRenderer, oldR, oldG, oldB, oldA);
	restoreRenderTarget(pOldTarget);

}

void GraphicsSystem::renderRect(GraphicsBuffer& dest, const Vector2D& destLoc, int width, int height, const Color& color, bool filled)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;

	SDL_Rect rect = { (int)destLoc.getX(), (int)destLoc.getY(), width, height };

	short int x = (short int)destLoc.getX();
	short int y = (short int)destLoc.getY();
	short int w = (short int)width;
	short int h = (short int)height;
	short int vx[4] = { x, x + w, x + w, x };
	short int vy[4] = { y,  y, y + h, y + h };

	if (filled)
	{
		int retval = filledPolygonRGBA(pRenderer, vx, vy, 4, (Byte)color.getR(), (Byte)color.getG(), (Byte)color.getB(), (Byte)color.getA());
	}
	else
	{
		polygonRGBA(pRenderer, vx, vy, 4, (Byte)color.getR(), (Byte)color.getG(), (Byte)color.getB(), (Byte)color.getA());
	}

	restoreRenderTarget(pOldTarget);

}

void GraphicsSystem::renderCircle(GraphicsBuffer& dest, const Vector2D& destLoc, int radius, const Color& color, bool filled)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;

	if (filled)
	{
		int retval = filledCircleRGBA(pRenderer, (short int)destLoc.getX(), (short int)destLoc.getY(), (short int)radius, (Byte)color.getR(), (Byte)color.getG(), (Byte)color.getB(), (Byte)color.getA());
	}
	else
	{
		int retval = circleRGBA(pRenderer, (short int)destLoc.getX(), (short int)destLoc.getY(), (short int)radius, (Byte)color.getR(), (Byte)color.getG(), (Byte)color.getB(), (Byte)color.getA());
	}
	restoreRenderTarget(pOldTarget);
}

void GraphicsSystem::renderLine(GraphicsBuffer& dest, const Vector2D& start, const Vector2D& end, const Color& color)
{
	SDL_Texture* pOldTarget = setRenderTarget(dest);
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;

	int retval = lineRGBA(pRenderer, (short int)start.getX(), (short int)start.getY(), (short int)end.getX(), (short int)end.getY(), (Byte)color.getR(), (Byte)color.getG(), (Byte)color.getB(), (Byte)color.getA());
	restoreRenderTarget(pOldTarget);
	
}

bool GraphicsSystem::isInside(const Vector2D& point, const Vector2D& spriteLoc, const Sprite& sprite)
{
	Vector2D lowerRight(spriteLoc.getX() + sprite.getWidth(), spriteLoc.getY() + sprite.getHeight());
	return isInside(point, spriteLoc, lowerRight);
}

bool GraphicsSystem::isInside(const Vector2D& point, const Vector2D& upperLeftCorner, const Vector2D& lowerRightCorner)
{

	float x = point.getX();
	float y = point.getY();
	float minX = upperLeftCorner.getX();
	float minY = upperLeftCorner.getY();
	float maxX = lowerRightCorner.getX();
	float maxY = lowerRightCorner.getY();

	if (x<minX || x>maxX || y<minY || y>maxY)
		return false;
	else
		return true;
}

bool GraphicsSystem::isInside(const Vector2D& point, const Vector2D& upperLeftCorner, Uint32 width, Uint32 height)
{
	Vector2D lowerRight(upperLeftCorner.getX() + width, upperLeftCorner.getY() + height);
	return isInside(point, upperLeftCorner, lowerRight);
}

Color GraphicsSystem::lerpColor(const Color& low, const Color& high, double pct)
{
	int lr = low.getR();
	int lg = low.getG();
	int lb = low.getB();
	int hr = high.getR();
	int hg = high.getG();
	int hb = high.getB();
	int r = lerp(lr, hr, pct);
	int g = lerp(lg, hg, pct);
	int b = lerp(lb, hb, pct);
	return Color(r, g, b);
}


SDL_Color GraphicsSystem::getSDLColorFromColor(const Color& color)
{
	SDL_Color theSDLColor;
	theSDLColor.r = color.getR();
	theSDLColor.g = color.getG();
	theSDLColor.b = color.getB();
	theSDLColor.a = color.getA();
	return theSDLColor;
}

Uint8 GraphicsSystem::getIntAlphaValue(double alpha)
{
	Uint8 val = (Uint8)(255 * alpha);
	return val;
}

double GraphicsSystem::getFloatAlphaValue(Uint8 alpha)
{
	double val = alpha / 255.0;
	return val;
}

SDL_Texture* GraphicsSystem::loadTexture(const std::string& filename)
{
	SDL_Surface* pSurface = IMG_Load(filename.c_str());

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(mpRenderer->mpRenderer, pSurface);

	SDL_FreeSurface(pSurface);
	return pTexture;
}

SDL_Texture* GraphicsSystem::createTexture(int width, int height)
{
	SDL_Texture* pTexture = SDL_CreateTexture(mpRenderer->mpRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	return pTexture;
}

SDL_Texture* GraphicsSystem::setRenderTarget(GraphicsBuffer& buffer)
{
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_Texture* pOldTarget = SDL_GetRenderTarget(pRenderer);
	SDL_SetRenderTarget(pRenderer, buffer.mpTexture);
	return pOldTarget;
}

void GraphicsSystem::restoreRenderTarget(SDL_Texture* oldTarget)
{
	SDL_Renderer* pRenderer = mpRenderer->mpRenderer;
	SDL_SetRenderTarget(pRenderer, oldTarget);
}
