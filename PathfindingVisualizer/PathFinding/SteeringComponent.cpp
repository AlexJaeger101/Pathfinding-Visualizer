#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "MoveToSteering.h"



SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

std::string SteeringComponent::getSteeringDescription()
{
	if (mpSteering)
		return mpSteering->getDescription();
	else
		return std::string();
}

void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;
	Steering* pOldSteering = mpSteering;//keep old steering so we can delete it later

	switch (data.type)
	{
		case Steering::INVALID_TYPE:
		{
			//create new steering
			mpSteering = NULL;
			break;
		}
		case Steering::MOVE_TO:
		{
			//create new steering
			mpSteering = new MoveToSteering(data.ownerID, data.targetLoc, data.targetID);
			break;
		}
		default:
		{
			assert(false);
		}
	};
	if (mpSteering != pOldSteering)//new steering was actually allocated
	{
		delete pOldSteering;
	}
}

