#include "FontManager.h"
#include "Font.h"
#include <iostream>
#include <cassert>

using namespace std;

void FontManager::clear()
{
	for (auto iter : mpFonts)
	{
		delete iter.second;
	}
	mpFonts.clear();
}

Font* FontManager::createAndManageFont(const IDType& id, const std::string& filename, Uint32 size)
{
	Font* pFont = nullptr;
	//already here?
	auto iter = mpFonts.find(id);

	if (iter != mpFonts.end())//found
	{
		cout << "trying to add a font which already exists";
		assert(false);
	}
	else
	{
		pFont = new Font(filename, size);
		mpFonts[id] = pFont;
	}

	return pFont;
}

Font* FontManager::getFont(const IDType& id)
{
	auto iter = mpFonts.find(id);

	if (iter != mpFonts.end())//found
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

void FontManager::destroyFont(const IDType& id)
{
	auto iter = mpFonts.find(id);

	if (iter != mpFonts.end())//found
	{
		delete iter->second;
		mpFonts.erase(iter);
	}
}