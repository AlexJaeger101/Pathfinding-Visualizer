#pragma once

#include <Trackable.h>
#include "Defines.h"
#include <unordered_map>
#include <string>

class Font;

class FontManager : public Trackable
{
public:
	FontManager() {};
	~FontManager() { clear(); };

	void clear();

	Font* createAndManageFont(const IDType& id, const std::string& filename, Uint32 size);
	Font* getFont(const IDType& id);
	void destroyFont(const IDType& id);

private:
	std::unordered_map<IDType, Font*> mpFonts;
};