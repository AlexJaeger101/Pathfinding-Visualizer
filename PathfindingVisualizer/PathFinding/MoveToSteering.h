#pragma once

#include <Trackable.h>
#include "Steering.h"

class MoveToSteering : public Steering
{
public:
	MoveToSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID=INVALID_UNIT_ID, float minDist = 5.0f);

protected:
	virtual Steering* getSteering();

	float mMinDistToTargetSquared = 25.0f;//5 pixels
};