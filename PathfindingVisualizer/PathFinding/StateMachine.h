#pragma once

#include <Trackable.h>
#include <vector>

class SMTransition;
class SMState;
class StateMachine;

enum SMStateType
{
	INVALID_SM_STATE = -1,
	END_STATE,
	NUM_SM_STATES
};

class SMState :public Trackable
{
public:
	SMState(SMStateType type) :mType(type) {};
	virtual ~SMState() {};

	virtual void onEntrance() = 0;
	virtual void onExit() = 0;
	virtual SMState* update(double elapsedTime) = 0;

	void addTransition(SMTransition* pTransition);

	SMStateType getType() { return mType; };

private:
	SMStateType mType;
	std::vector<SMTransition*> mTransitions;
};

class EndSMState :public SMState
{
public:
	EndSMState():SMState(END_STATE) {};
	virtual void onEntrance() {};
	virtual void onExit() {};
	virtual SMState* update(double elapsedTime) { return nullptr; };

};

class SMTransition :public Trackable
{
public:
	SMTransition(SMState* pState) :mTransitionTo(pState) {};
	virtual bool shouldTransition() const=0;
private:
	SMState* mTransitionTo = nullptr;
};

class StateMachine :public Trackable
{
public:
	StateMachine() {};
	virtual ~StateMachine();

	bool update(double elapsedTime);//true means machine still running, false means machine in end state
	virtual void start()=0;
protected:
	SMState* mpCurrentState = nullptr;
	std::vector<SMTransition*> mAllTransitions;
	std::vector<SMState*> mStates;
};
