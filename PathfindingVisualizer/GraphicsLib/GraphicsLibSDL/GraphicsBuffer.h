#pragma once

#include <Trackable.h>

#include <string>
#include "Color.h"

class Vector2D;
class GraphicsSystem;
class GraphicsBufferManager;
class Renderer;
class System;
class InputSystem;
struct SDL_Texture;
struct SDL_Renderer;

const int INVALID_DIMENSION = -1;

class GraphicsBuffer :public Trackable
{
public:
	friend class GraphicsSystem;
	friend class Renderer;
	friend class GraphicsBufferManager;
	friend class System;
	friend class InputSystem;

	//accessors
	int getWidth() const;
	int getHeight() const;
	Vector2D getSize() const;
	void getDimensions(int& width, int& height) const;
	bool isValid() const { return mValid; };

private:
	SDL_Texture* mpTexture;
	bool mValid = false;

	GraphicsBuffer(const Renderer* pRenderer);//used to create the back buffer
	GraphicsBuffer(SDL_Texture* pTexture);
	GraphicsBuffer(Renderer* pRenderer, unsigned int width, unsigned int height, const Color& color = BLACK_COLOR);

	~GraphicsBuffer();//to be called by GraphicsBufferManager
	GraphicsBuffer* clone(Renderer* pRenderer) const;//to be called by GraphicsBufferManager

	//invalidate copy constructor and assignment operator
	GraphicsBuffer(const GraphicsBuffer& rhs);
	GraphicsBuffer& operator=(const GraphicsBuffer& rhs);

};
