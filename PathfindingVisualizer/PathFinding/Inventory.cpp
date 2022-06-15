#include "Inventory.h"
#include "Game.h"
#include <GraphicsSystem.h>
#include <FontManager.h>
#include <Font.h>
#include <cassert>
#include <string>

using namespace std;

Inventory::Inventory(const std::vector<Item*>& items, Uint32 maxItems)
	:mItems(items)
	,mMaxNumItems(maxItems)
{
}

Inventory::Inventory(Uint32 maxItems)
	:mMaxNumItems(maxItems)
{
}

Inventory::~Inventory()
{
	clear();
}

bool Inventory::addItem(Item* pItem)
{
	if (hasSlotsRemaining())
	{
		mItems.push_back(pItem);
		return true;
	}
	else
	{
		return false;
	}
}

Item* Inventory::getAndRemoveItem(ItemType type)
{
	Item* pItem = nullptr;
	
	for (Uint32 i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->getType() == type || type==ItemType::INVALID)
		{
			pItem = mItems[i];
			mItems.erase(mItems.begin() + i);
			break;
		}
	}

	return pItem;
}

const Item* Inventory::peekAtItem(ItemType type) const
{
	Item* pItem = nullptr;

	for (Uint32 i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->getType() == type || type == ItemType::INVALID)
		{
			pItem = mItems[i];
			break;
		}
	}

	return pItem;
}

void Inventory::clear()
{
	for (auto it : mItems)
	{
		delete it;
	}
	mItems.clear();
}

void Inventory::draw(const Vector2D& loc) const
{
	Font* pFont = gpGame->getFontManager()->getFont(COUR_14_FONT_ID);
	assert(pFont);
	string theString = createDisplayString();
	GraphicsSystem::writeText(loc, *pFont, RED_COLOR, theString);
}

bool Inventory::hasItemOfType(ItemType type) const
{
	for (Uint32 i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->getType() == type)
		{
			return true;
		}
	}

	return false;
}

Uint32 Inventory::getNumItemsOfType(ItemType type) const
{
	Uint32 cnt = 0;
	for (Uint32 i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->getType() == type)
		{
			cnt++;
		}
	}
	return cnt;
}

bool Inventory::hasAllItemsForRecipe(const Recipe& recipe) const
{
	for (int type = (int)ItemType::INVALID; type < (int)ItemType::NUM_ITEM_TYPES; type++)
	{
		if (getNumItemsOfType((ItemType)type) < recipe.getNumRequired((ItemType)type))
			return false;
	}
	return true;
}

Uint32 Inventory::getNumSlotsRemaining() const
{
	return mMaxNumItems - mItems.size();
}

Uint32 Inventory::getNumSlotsFilled() const
{
	return mItems.size();
}

bool Inventory::hasSlotsRemaining() const
{
	if (getNumSlotsRemaining() > 0)
		return true;
	else
		return false;
}

std::string Inventory::getDisplayString() const
{
	return createDisplayString();
}

std::string Inventory::createDisplayString() const
{
	string theString;
	for (Uint32 i = 0; i < mItems.size(); i++)
	{
		theString += ITEM_CHARS[(Uint32)mItems[i]->getType()];
	}
	return theString;
}
