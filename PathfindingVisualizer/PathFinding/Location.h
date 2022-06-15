#pragma once

#include <Trackable.h>
#include <Vector2D.h>
#include <Sprite.h>
#include "Item.h"
#include "Recipe.h"
#include "Inventory.h"
#include "GameDefines.h"
#include <string>
#include <vector>

/*struct ProductionRecord :public Trackable
{
	ItemType input;
	ItemType output;
	double timeToProduce;
	bool requiresUnit;

	bool operator==(const ProductionRecord& rhs) const;
	bool operator!=(const ProductionRecord& rhs) const;

	ProductionRecord(ItemType in, ItemType out, double productionTime, bool needsUnit = true)
		:input(in), output(out), timeToProduce(productionTime), requiresUnit(needsUnit) {};
};*/

//const ProductionRecord NULL_PRODUCTION_RECORD(ItemType::INVALID, ItemType::INVALID, 0.0);

struct ActiveProduction :public Trackable
{
	std::vector<Item*> inputs;
	ItemType output;
	double timeToProduce;
	double timeLeft;
	int VPs;
	bool requiresUnit;

	ActiveProduction(std::vector<Item*> in, ItemType out, double productionTime, int points, bool needsUnit = true)
		:inputs(in), output(out), timeToProduce(productionTime), timeLeft(productionTime), VPs(points), requiresUnit(needsUnit) {};

	bool operator==(const ActiveProduction& rhs) const;
	bool operator!=(const ActiveProduction& rhs) const;
};

const ActiveProduction NULL_ACTIVE_PRODUCTION(std::vector<Item*>(), ItemType::INVALID, 0.0, 0);

class Location :public Trackable
{
public:
	void clear();

	void setRecipe(const Recipe& recipe);
	const Recipe& getRecipe() const;

	bool isItemInInventory(ItemType type) const;
	Item* getItemFromInventory(ItemType type=ItemType::INVALID);//removes item from inventory - Invalid item type means just get an item - don't care about type
	const Item* peekAtItemFromInventory(ItemType type = ItemType::INVALID) const;
	bool addItemToInventory(Item* pItem);//returns false if no room in inventory
	Uint32 getNumItemsInInventory() const;

	void update(double dt);
	void draw() const;

	bool startProduction(ItemType typeToProduce);
	bool hasActiveProduction() const;
	bool hasUnitInside() const;
	ItemType getActiveProductionItem() const;
	double getTimeLeftForActiveProduction() const;//return 0.0 if no active production of given type 
	bool hasRoomInInventory() const;
	void setPosition(const Vector2D& pos) { mPos = pos; };
	const Vector2D& getPosition() const { return mPos; };
	const Sprite& getSprite() const { return mSprite; };
	bool hasAllItemsForRecipe() const { return mInventory.hasAllItemsForRecipe(mRecipe); };

	std::string getName() const { return mName; };
	LocationID getID() const { return mID; };

private:
	Location(const LocationID& id,const Sprite& sprite, const Vector2D& pos, std::string name, const Recipe& recipe, Uint32 maxInventory = 5);
	~Location();

	friend class LocationManager;

	std::string mName = "Unknown";
	LocationID mID = INVALID_LOCATION_ID;
	Recipe mRecipe;//what can be produced
	ActiveProduction mActive= NULL_ACTIVE_PRODUCTION;//what is currently being produced
	Inventory mInventory;
	Vector2D mPos;
	Sprite mSprite;

	void deleteCompletedActiveProduction();
	std::string Location::createDisplayString() const;
};