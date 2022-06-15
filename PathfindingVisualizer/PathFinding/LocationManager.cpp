#include "LocationManager.h"
#include "Location.h"
#include <GraphicsSystem.h>

LocationManager::~LocationManager()
{
	clear();
}

void LocationManager::clear()
{
	for (auto it : mLocations)
	{
		delete it.second;
	}
	mLocations.clear();
}

Location* LocationManager::createLocation(const LocationID& id, Sprite sprite, const Vector2D& pos, const std::string& name, const Recipe& recipe, Uint32 maxInventory)
{
	Location* location = new Location(id, sprite, pos, name, recipe, maxInventory);
	auto it = mLocations.find(id);
	if (it == mLocations.end())//not already in map
	{
		mLocations[id] = location;
	}
	return location;
}

Location* LocationManager::getLocation(const LocationID& id)
{
	auto it = mLocations.find(id);
	if (it != mLocations.end())//found
	{
		return it->second;
	}
	else
		return nullptr;
}

void LocationManager::updateAll(double dt)
{
	for (auto it : mLocations)
	{
		it.second->update(dt);
	}
}

void LocationManager::drawAll() const
{
	for (auto it : mLocations)
	{
		it.second->draw();
	}

}

std::vector<Location*> LocationManager::getLocationsThatProduceItemType(ItemType type) const
{
	std::vector<Location*> locations;
	for (auto it : mLocations)
	{
		if (it.second->getRecipe().getOutputType() == type)
			locations.push_back(it.second);
	}

	return locations;
}

std::vector<Location*> LocationManager::getLocationsThatContainItem(ItemType type) const
{
	std::vector<Location*> locations;
	for (auto it : mLocations)
	{
		if (it.second->isItemInInventory(type) )
			locations.push_back(it.second);
	}

	return locations;
}

Location* LocationManager::getIntersectingLocation(const Vector2D& pos)
{
	for (auto it : mLocations)
	{
		Location* pLocation = it.second;
		if (GraphicsSystem::isInside(pos, pLocation->getPosition(), pLocation->getSprite()))
			return pLocation;
	}
	return nullptr;
}
