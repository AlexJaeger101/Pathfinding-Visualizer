#pragma once

#include <Trackable.h>
#include <string>
#include <DeanLibDefines.h>
#include "SDL_FontCache.h"

class GraphicsSystem;

class Font:public Trackable
{
	friend class GraphicsSystem;
	friend class FontManager;
public:
	enum Alignment
	{
		LEFT = FC_ALIGN_LEFT,
		CENTER = FC_ALIGN_CENTER,
		RIGHT = FC_ALIGN_RIGHT
	};

	Uint32 getSize() const { return mSize; };

private:
	FC_Font* mpFont;
	Uint32 mSize;

	FC_Font* getFont() const { return mpFont; };

	Font(const std::string& filename, Uint32 size);
	~Font();

	//invalidate copy constructor and assignment operator
	Font(const Font& rhs);
	Font& operator=(const Font& rhs);

};
