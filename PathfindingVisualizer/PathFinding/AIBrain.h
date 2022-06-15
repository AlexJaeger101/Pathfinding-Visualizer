#pragma once

#include <Trackable.h>

class Unit;
class StateMachine;

class AIBrain :public Trackable
{
public:
	enum BrainType
	{
		DEFAULT,
		RANDOM
	};

	AIBrain(Unit* pUnit, BrainType type=DEFAULT);
	virtual ~AIBrain();

	virtual void update(double dt);
	BrainType getType()const { return mType; };
private:
	Unit* mpUnit;
	BrainType mType;
	StateMachine* mpMachine = nullptr;
};
