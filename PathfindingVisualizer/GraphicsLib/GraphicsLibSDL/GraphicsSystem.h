#pragma once

#include <SDL.h>
#include <Trackable.h>
#include <Vector2D.h>

#include "Color.h"
#include "Font.h"
#include "Defines.h"

class GraphicsBuffer;
class Sprite;
class System;
class Renderer;
class Font;

class GraphicsSystem :public Trackable
{
public:
	static unsigned int getDisplayWidth() { return mWidth; };
	static unsigned int getDisplayHeight() { return mHeight; };
	static GraphicsBuffer* getBackBuffer() { return mpBackBuffer; };
	static Renderer* getRenderer() { return mpRenderer; };

	void flip();

	static void draw(const Vector2D& destLoc, const GraphicsBuffer& src, double rotationInRadians = 0.0, double scale = 1.0, double alpha=1.0, const Color& color=WHITE_COLOR);
	static void draw(GraphicsBuffer& dest, const Vector2D& loc, const GraphicsBuffer& src, double rotationInRadians = 0.0, double scale = 1.0, double alpha = 1.0, const Color& color = WHITE_COLOR);
	static void draw(const Vector2D& destLoc, const Sprite& sprite, double rotationInRadians = 0.0, double scale = 1.0, double alpha = 1.0, const Color& color = WHITE_COLOR);
	static void draw(GraphicsBuffer& dest, const Vector2D& destLoc, const Sprite& sprite, double rotationInRadians = 0.0, double scale = 1.0, double alpha = 1.0, const Color& color = WHITE_COLOR);
	static void writeText(GraphicsBuffer& dest, const Vector2D& destLoc, const Font& font, const Color& color, const std::string& text, Font::Alignment align = Font::LEFT);
	static void writeText(const Vector2D& destLoc, const Font& font, const Color& color, const std::string& text, Font::Alignment align = Font::LEFT);

	static void setBufferToColor(GraphicsBuffer& buffer, const Color& color);
	static void saveBufferToFile(const GraphicsBuffer& buffer, const std::string& filename);

	static void renderFilledRect(GraphicsBuffer& dest, const Vector2D& destLoc, int width, int height, const Color& color);//deprecated
	static void renderRect(GraphicsBuffer& dest, const Vector2D& destLoc, int width, int height, const Color& color, bool filled=true);
	static void renderCircle(GraphicsBuffer& dest, const Vector2D& destLoc, int radius, const Color& color, bool filled = true);
	static void renderLine(GraphicsBuffer& dest, const Vector2D& start, const Vector2D& end, const Color& color);

	static bool isInside(const Vector2D& point, const Vector2D& spriteLoc, const Sprite& sprite);
	static bool isInside(const Vector2D& point, const Vector2D& upperLeftCorner, const Vector2D& lowerRightCorner);
	static bool isInside(const Vector2D& point, const Vector2D& upperLeftCorner, Uint32 width, Uint32 height);

	static Color lerpColor(const Color& low, const Color& high, double pct);
private:
	GraphicsSystem();
	~GraphicsSystem();

	bool init(unsigned int width, unsigned int height);
	void cleanup();

	SDL_Window* mpWindow = NULL;
	static Renderer* mpRenderer;
	static GraphicsBuffer* mpBackBuffer;
	static unsigned int mWidth;
	static unsigned int mHeight;
	bool mIsInitted = false;

	friend class System;
	friend class GraphicsBufferManager;

	static SDL_Texture* loadTexture(const std::string& filename);
	static SDL_Texture* createTexture(int width, int height);
	static SDL_Texture* setRenderTarget(GraphicsBuffer& pBuffer);//returns previous render target texture
	static void restoreRenderTarget(SDL_Texture* oldTarget);

	static SDL_Color getSDLColorFromColor(const Color& color);
	static Uint8 getIntAlphaValue(double alpha);
	static double getFloatAlphaValue(Uint8 alpha);

};
