#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <Trackable.h>
#include <Sprite.h>

#include "GameDefines.h"
#include "Location.h"
#include "Recipe.h"

class Location;

class LocationManager :public Trackable
{
public:
	LocationManager() {};
	~LocationManager();

	void clear();

	Location* createLocation(const LocationID& id, Sprite sprite, const Vector2D& pos, const std::string& name, const Recipe& recipe, Uint32 maxInventory = 5);
	Location* getLocation(const LocationID& id);
	void updateAll(double dt);
	void drawAll() const;
	std::vector<Location*> getLocationsThatProduceItemType(ItemType type) const;
	std::vector<Location*> getLocationsThatContainItem(ItemType type) const;

	Location* getIntersectingLocation(const Vector2D& pos);//return nullptr if none
private:
	std::unordered_map<LocationID, Location*> mLocations;
};
