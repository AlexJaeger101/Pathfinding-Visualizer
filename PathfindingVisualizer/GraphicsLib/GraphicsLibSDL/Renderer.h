#pragma once

#include "Trackable.h"
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class Font;
struct SDL_Window;
struct SDL_Renderer;

class Renderer :public Trackable
{
public:

	GraphicsBuffer* makeBackBuffer() const;

	//Renderer(GraphicsBuffer* pBuffer);
	~Renderer();


private:
	SDL_Renderer* mpRenderer;


	friend class GraphicsSystem;
	friend class GraphicsBuffer;
	friend class Font;
	friend class System;

	Renderer(SDL_Window* pWindow);

	//invalidate copy constructor and assignment operator
	Renderer(const Renderer& rhs);
	Renderer& operator=(const Renderer& rhs);

};