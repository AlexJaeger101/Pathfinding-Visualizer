#include <cassert>

#include "Steering.h"
#include "MoveToSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


MoveToSteering::MoveToSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, float minDist)
	: Steering(MOVE_TO,ownerID,targetLoc,targetID)
	, mMinDistToTargetSquared(minDist*minDist)
{
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	if (pTarget)
	{
		mTargetLoc = pTarget->getPosition();
	}
}

Steering* MoveToSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//uncomment to steer towards the current location of the taget rather than towards the location when the steering was created
	/*Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	if (pTarget)
	{
		mTargetLoc = pTarget->getPosition();
	}*/


	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (diff.getLengthSquared() <= mMinDistToTargetSquared)
	{
		data.vel = ZERO_VECTOR2D;
		data.acc = ZERO_VECTOR2D;
	}
	else
	{
		diff.normalize();
		diff *= pOwner->getMaxSpeed();
		//diff *= pOwner->getMaxAcc();//use acceleration to get smoother starts

		data.vel = diff;
		//data.acc = diff;
	}

	this->mData = data;
	return this;
}

