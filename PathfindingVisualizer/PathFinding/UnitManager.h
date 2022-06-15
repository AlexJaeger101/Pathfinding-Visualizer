#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <unordered_map>
#include <vector>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"
#include "AIBrain.h"

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;


class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager();
	void clear();

	Unit* createUnit(const Sprite& sprite, AIBrain::BrainType brainType = AIBrain::DEFAULT, bool shouldWrap = true, const PositionData& posData = PositionComponent::getZeroPositionData(), const PhysicsData& physicsData = PhysicsComponent::getZeroPhysicsData(), const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = PositionComponent::getZeroPositionData(), const PhysicsData& physicsData = PhysicsComponent::getZeroPhysicsData());
	Unit* createRandomUnit(const Sprite& sprite);

	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(double elapsedTime);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	Unit* getUnitAtPoint(const Vector2D& pt)const;
	Unit* getUnitInside(const Vector2D& spriteLoc, const Sprite& sprite) const;

	Uint32 getNumUnits() const { return mUnitMap.size(); };
	std::vector<Unit*> getUnitsInRangeFromUnit(Unit* pUnit, float range, bool includeUnit = false);
	static Vector2D calcAveragePosition(const std::vector<Unit*>& units);
	static Vector2D calcAverageHeading(const std::vector<Unit*>& units);

private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	std::unordered_map<UnitID, Unit*> mUnitMap;
};

