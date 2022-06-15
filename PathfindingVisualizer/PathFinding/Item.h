#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>

enum class ItemType
{
	INVALID = -1,
	FOOD,
	ORE,
	TOOLS,
	GOLD,
	DEPOSIT,
	NUM_ITEM_TYPES
};

const char ITEM_CHARS[(Uint32)ItemType::NUM_ITEM_TYPES] = { 'f', 'o', 't', 'g', 'd' };


class Item:public Trackable
{
public:
	Item(ItemType type);

	ItemType getType() const;

private:
	ItemType mType;

};