#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

UnitManager::~UnitManager()
{
	clear();
}

void UnitManager::clear()
{
	while (mUnitMap.size() > 0)
	{
		deleteUnit(mUnitMap.begin()->first);
	}
}

Unit* UnitManager::createUnit(const Sprite& sprite, AIBrain::BrainType brainType, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mpPhysicsComponent = pComponentManager->getPhysicsComponent(id);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);
		pUnit->mpSteeringComponent = pComponentManager->getSteeringComponent(id);

		//set max's
		pUnit->mMaxSpeed = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_SPEED).getFloatVal();
		pUnit->mMaxAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ACC).getFloatVal();
		pUnit->mMaxRotAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_ACC).getFloatVal();
		pUnit->mMaxRotVel = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_VEL).getFloatVal();

		pUnit->changeBrain(brainType);
	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, AIBrain::DEFAULT, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	int posX = rand() % GraphicsSystem::getDisplayWidth();
	int posY = rand() % GraphicsSystem::getDisplayHeight();
	//int velX = rand() % 50 - 25;
	//int velY = rand() % 40 - 20;
	float velX = 0.0f;
	float velY = 0.0f;
	Unit* pUnit = createUnit(sprite, AIBrain::DEFAULT, true, PositionData(Vector2D(posX,posY),0), PhysicsData(Vector2D(velX,velY),Vector2D(0.0f,0.0f), 0.0f, 0.00f));
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		//pUnit->setSteering(Steering::ARRIVE, Vector2D((int)(GraphicsSystem::getDisplayWidth() / 2), (int)(GraphicsSystem::getDisplayHeight() / 2)));
		pUnit->setSteering(Steering::INVALID_TYPE);
	}
	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				//failsafe against deleting player unit
				if( getPlayerUnit() != it->second )
					deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(double elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

Unit* UnitManager::getUnitAtPoint(const Vector2D& pt) const
{
	for (auto iter : mUnitMap)
	{
		Unit* pUnit = iter.second;
		if (GraphicsSystem::isInside(pt, pUnit->getPosition(), pUnit->getCurrentSprite()))
			return pUnit;
	}

	return nullptr;
}

Unit* UnitManager::getUnitInside(const Vector2D& spriteLoc, const Sprite& sprite) const
{
	for (auto iter : mUnitMap)
	{
		Unit* pUnit = iter.second;
		if (GraphicsSystem::isInside(pUnit->getPosition(), spriteLoc, sprite))
			return pUnit;
	}

	return nullptr;
}

std::vector<Unit*> UnitManager::getUnitsInRangeFromUnit(Unit* pUnit, float range, bool includeUnit)
{
	std::vector<Unit*> units;

	float rangeSquared = range * range;
	Vector2D loc = pUnit->getPositionComponent()->getPosition();

	for (auto iter : mUnitMap)
	{
		Unit* pOtherUnit = iter.second;
		if (includeUnit || pOtherUnit != pUnit)
		{
			Vector2D diff = loc - pOtherUnit->getPositionComponent()->getPosition();
			if (diff.getLengthSquared() <= rangeSquared)
			{
				units.push_back(pOtherUnit);
			}
		}
	}
	return units;;
}

Vector2D UnitManager::calcAveragePosition(const std::vector<Unit*>& units)
{
	Vector2D sum;

	for (auto pUnit : units)
	{
		sum += pUnit->getPositionComponent()->getPosition();
	}

	UINT numUnits = units.size();
	if (numUnits > 0)
	{
		sum /= (float)numUnits;
	}
	return sum;
}

Vector2D UnitManager::calcAverageHeading(const std::vector<Unit*>& units)
{
	Vector2D sum;

	for (auto pUnit : units)
	{
		sum += pUnit->getHeadingVector();
	}

	UINT numUnits = units.size();
	if (numUnits > 0)
	{
		sum /= (float)numUnits;
	}
	return sum;
}
