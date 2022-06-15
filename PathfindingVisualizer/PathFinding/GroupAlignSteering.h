#pragma once

#include "Steering.h"

class GroupAlignSteering : public Steering
{
public:
	GroupAlignSteering(const UnitID& ownerID);

private:
	Steering* getSteering();
};
