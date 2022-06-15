#include "StateMachine.h"

StateMachine::~StateMachine()
{
	{ delete mpCurrentState; }
}

bool StateMachine::update(double elapsedTime)
{
	if (mpCurrentState)
	{
		SMState* pNewState = mpCurrentState->update(elapsedTime);
		if (pNewState)
		{
			//exit old state
			mpCurrentState->onExit();

			//set new state to be the current state
			mpCurrentState = pNewState;

			//enter new state
			mpCurrentState->onEntrance();
		}
	}
	else
	{
		start();
	}

	if (mpCurrentState && mpCurrentState->getType() == END_STATE)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void SMState::addTransition(SMTransition* pTransition)
{
	mTransitions.push_back(pTransition);
}
