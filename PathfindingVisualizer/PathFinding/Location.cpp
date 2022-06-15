#include "Location.h"
#include <cassert>
#include <GraphicsSystem.h>
#include "Game.h"
#include "GameEvent.h"
#include "UnitManager.h"
#include <FontManager.h>
#include <EventSystem.h>

using namespace std;

Location::Location(const LocationID& id, const Sprite& sprite, const Vector2D& pos, std::string name, const Recipe& recipe, Uint32 maxInventory)
	:mSprite(sprite)
	,mPos(pos)
	,mName(name)
	,mID(id)
	,mRecipe(recipe)
	,mInventory(maxInventory)
{
}

Location::~Location()
{
	clear();
}

void Location::clear()
{
	for (Uint32 i = 0; i < mActive.inputs.size(); i++)
	{
		delete mActive.inputs[i];
	}
	mActive.inputs.clear();

	mInventory.clear();
}

void Location::setRecipe(const Recipe& recipe)
{
	mRecipe = recipe;
}

const Recipe& Location::getRecipe() const
{
	return mRecipe;
}

bool Location::isItemInInventory(ItemType type) const
{
	return mInventory.hasItemOfType(type);
}

Item* Location::getItemFromInventory(ItemType type)//Invalid item type means just get an item - don't care about type
{
	return mInventory.getAndRemoveItem(type);
}

const Item* Location::peekAtItemFromInventory(ItemType type) const
{
	return mInventory.peekAtItem(type);
}

bool Location::addItemToInventory(Item* pItem)
{
	return mInventory.addItem(pItem);
}

Uint32 Location::getNumItemsInInventory() const
{
	return mInventory.getNumSlotsFilled();
}

void Location::update(double dt)
{
	//go through all active productions and advance their time spent
	if (mActive != NULL_ACTIVE_PRODUCTION && (!mActive.requiresUnit || hasUnitInside()))
	{
		mActive.timeLeft -= dt;
		if (mActive.timeLeft <= 0.0)
		{
			if (mActive.output == ItemType::DEPOSIT)//just a deposit - do nothing for now
			{
			}
			else //actually producing an Item
			{
				Item* pItem = new Item(mActive.output);
				mInventory.addItem(pItem);
			}
			for (Uint32 i = 0; i < mActive.inputs.size(); i++)
			{
				delete mActive.inputs[i];
			}
			EventSystem::getInstance()->fireEvent(ScorePointsEvent(mActive.VPs));
			mActive.inputs.clear();
			mActive = NULL_ACTIVE_PRODUCTION;
		}
	}

	if (mInventory.hasSlotsRemaining() && mActive == NULL_ACTIVE_PRODUCTION)
	{
		if (mRecipe.getNumInputs() == 0 && (!mRecipe.requiresUnit() || hasUnitInside()))
		{
			startProduction(mRecipe.getOutputType());
		}
	}
}

void Location::draw() const
{
	GraphicsSystem::draw(mPos, mSprite);
	Font* pFont = gpGame->getFontManager()->getFont(COUR_14_FONT_ID);
	assert(pFont);
	string theString = createDisplayString();

	GraphicsSystem::writeText(mPos, *pFont, RED_COLOR, theString);
}

bool Location::startProduction(ItemType typeToProduce)
{
	if (mRecipe.getOutputType() == typeToProduce)
	{
		if (mRecipe.getNumInputs() == 0)//doesn't actually require anything
		{
			mActive = ActiveProduction(vector<Item*>(), typeToProduce, mRecipe.getTimeToProduce(), mRecipe.getVPs(), mRecipe.requiresUnit());
			return true;
		}
		else if (mInventory.hasAllItemsForRecipe(mRecipe))
		{
			vector<Item*> items;
			for (int type = (int)ItemType::INVALID; type < (int)ItemType::NUM_ITEM_TYPES; type++)
			{
				Uint32 numRequired = mRecipe.getNumRequired((ItemType)type);
				for (Uint32 i = 0; i < numRequired; i++)
				{
					Item* pInputItem = mInventory.getAndRemoveItem((ItemType)type);
					assert(pInputItem);
					items.push_back(pInputItem);
				}
			}

			mActive = ActiveProduction(items, typeToProduce, mRecipe.getTimeToProduce(), mRecipe.getVPs(), mRecipe.requiresUnit());
			return true;
		}
	}
	return false;
}

bool Location::hasActiveProduction() const
{
	if (mActive != NULL_ACTIVE_PRODUCTION)
		return true;
	else
		return false;
}

bool Location::hasUnitInside() const
{
	Unit* pUnit = gpGame->getUnitManager()->getUnitInside(mPos,mSprite);
	if (pUnit)
		return true;
	else
		return false;
}

ItemType Location::getActiveProductionItem() const
{
	return mActive.output;
}

double Location::getTimeLeftForActiveProduction() const
{
	return mActive.timeLeft;
}

bool Location::hasRoomInInventory() const
{
	return (mInventory.getNumSlotsRemaining() > 0);
}

void Location::deleteCompletedActiveProduction()
{
	mActive = NULL_ACTIVE_PRODUCTION;
}

std::string Location::createDisplayString() const
{
	string theString;
	theString += mName;
	theString += "\nin:";
	theString += mInventory.getDisplayString();
	theString += "\npr:";
	theString += ITEM_CHARS[(Uint32)mActive.output];
	theString += " ";
	theString += to_string(mActive.timeLeft);

	theString += '\n';

	return theString;
}

bool ActiveProduction::operator==(const ActiveProduction& rhs) const
{
	if (inputs.size() != rhs.inputs.size())
		return false;
	for (Uint32 i = 0; i < inputs.size(); i++)
	{
		if (inputs[i]->getType() != rhs.inputs[i]->getType())
			return false;
	}

	if (output != rhs.output)
		return false;
	if (timeToProduce != rhs.timeToProduce)
		return false;
	if (VPs != rhs.VPs)
		return false;
	if (requiresUnit != rhs.requiresUnit)
		return false;

	return true;
}

bool ActiveProduction::operator!=(const ActiveProduction& rhs) const
{
	return !((*this) == rhs);
}
