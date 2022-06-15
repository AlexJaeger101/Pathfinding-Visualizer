#pragma once

#include <Trackable.h>
#include <Vector2D.h>
#include <DeanLibDefines.h>
#include <vector>
#include <string>
#include "Item.h"
#include "Recipe.h"


class Inventory :public Trackable
{
public:
	Inventory(const std::vector<Item*>& items, Uint32 maxItems);
	Inventory(Uint32 maxItems);
	~Inventory();

	bool addItem(Item* pItem);//return true if successfully added, false otherwise
	Item* getAndRemoveItem(ItemType type=ItemType::INVALID);//Invalid means don't care what type
	const Item* peekAtItem(ItemType type = ItemType::INVALID) const;//Invalid means don't care what type
	void clear();

	void draw(const Vector2D& loc) const;

	bool hasItemOfType(ItemType type) const;
	Uint32 getNumItemsOfType(ItemType type) const;
	bool hasAllItemsForRecipe(const Recipe& recipe) const;
	Uint32 getNumSlotsRemaining() const;
	Uint32 getNumSlotsFilled() const;
	bool hasSlotsRemaining() const;
	std::string getDisplayString() const;

private:
	std::vector<Item*> mItems;
	Uint32 mMaxNumItems;

	std::string createDisplayString() const;
};
