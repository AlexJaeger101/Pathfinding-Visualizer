#include "Item.h"

Item::Item(ItemType type)
	:mType(type)
{
}

ItemType Item::getType() const
{
	return mType;
}
