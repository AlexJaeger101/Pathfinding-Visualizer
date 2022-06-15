#include "GroupAlignSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include <cassert>


GroupAlignSteering::GroupAlignSteering(const UnitID& ownerID)
	:Steering(Steering::GROUP_ALIGN, ownerID)
{
}

Steering* GroupAlignSteering::getSteering()
{
	UnitManager* pUnitManager = gpGame->getUnitManager();
	Unit* pOwner = pUnitManager->getUnit(mOwnerID);

	float radius = gpGame->getDataRepository()->getEntry(DataKeyEnum::ALIGN_RADIUS).getFloatVal();

	std::vector<Unit*> units = pUnitManager->getUnitsInRangeFromUnit(pOwner, radius);
	Vector2D avgPos = UnitManager::calcAverageHeading(units);

	mData.acc = pOwner->getPositionComponent()->getPosition() - avgPos;

	return this;
}
