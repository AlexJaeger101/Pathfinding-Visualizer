#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
#include "Inventory.h"
#include "Item.h"
#include "AIBrain.h"

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;

class Unit : public Trackable
{
public:
	void draw() const;
	float getFacing() const;
	const Vector2D& getPosition() const;
	Vector2D getHeadingVector() const;

	virtual void update(double elapsedTime);

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);

	bool addItem(Item* pItem);//returns false if there is no room
	bool hasRoomInInventory() const;
	Item* getAndRemoveItem(ItemType type=ItemType::INVALID);
	const Item* peekAtItem(ItemType type = ItemType::INVALID);
	bool hasItemOfType(ItemType type) const;

	const UnitID& getID() const { return mID; };
	const Sprite& getCurrentSprite() const { return mSprite; };

	void changeBrain(AIBrain::BrainType type);
	AIBrain::BrainType getBrainType() const;

private:
	UnitID mID = INVALID_UNIT_ID;
	ComponentID mPhysicsComponentID = INVALID_COMPONENT_ID;
	ComponentID mPositionComponentID = INVALID_COMPONENT_ID;
	ComponentID mSteeringComponentID = INVALID_COMPONENT_ID;
	PositionComponent* mpPositionComponent = NULL;
	PhysicsComponent* mpPhysicsComponent = NULL;
	SteeringComponent* mpSteeringComponent = NULL;
	Sprite mSprite;
	float mMaxAcc = 0.0f;
	float mMaxSpeed = 0.0f;
	float mMaxRotAcc = 0.0f;
	float mMaxRotVel = 0.0f;
	Inventory mInventory;
	AIBrain* mpBrain = nullptr;
	bool mShowTarget = false;
	bool mCenterSprite = true;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&)=delete;//invalidate copy constructor
	void operator=(Unit&)=delete;//invalidate assignment operator

	friend class UnitManager;
};
