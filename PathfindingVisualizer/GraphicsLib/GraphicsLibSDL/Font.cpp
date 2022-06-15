#include "Font.h"
#include "SDL_FontCache.h"
#include "GraphicsSystem.h"
#include "Renderer.h"
#include <cassert>

using namespace std;

Font::Font(const std::string& filename, Uint32 size)
	:mSize(size)
{
	mpFont = FC_CreateFont();
	assert(mpFont);
	
	//color is ignored - will be applied when being drawn
	FC_LoadFont(mpFont, GraphicsSystem::getRenderer()->mpRenderer, filename.c_str(), size, FC_MakeColor(0, 0, 0, 0), TTF_STYLE_NORMAL);

}

Font::~Font()
{
	FC_FreeFont(mpFont);
}
