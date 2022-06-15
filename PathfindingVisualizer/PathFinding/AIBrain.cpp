#include "AIBrain.h"
#include "StateMachine.h"
#include "Unit.h"
#include <iostream>

using namespace std;

AIBrain::AIBrain(Unit* pUnit, BrainType type)
	:mpUnit(pUnit)
	,mType(type)
{
	switch (type)
	{
	case DEFAULT:
		break;
	}
}

AIBrain::~AIBrain()
{
	delete mpMachine;
}

void AIBrain::update(double dt)
{
}
