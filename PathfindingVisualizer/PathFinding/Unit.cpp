#include "Unit.h"
#include <assert.h>
#include <iostream>
#include <FontManager.h>
#include <Font.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "ComponentManager.h"
#include "SpriteManager.h"
#include "Location.h"
#include "LocationManager.h"


Unit::Unit(const Sprite& sprite)
	:mSprite(sprite)
	, mPositionComponentID(INVALID_COMPONENT_ID)
	, mPhysicsComponentID(INVALID_COMPONENT_ID)
	, mSteeringComponentID(INVALID_COMPONENT_ID)
	, mInventory(2)
	, mShowTarget(false)
{
}

Unit::~Unit()
{
	delete mpBrain;
}

void Unit::draw() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	float facing = (float)mapToRangeMinusPiToPi((double)pPosition->getFacing());

	if (mCenterSprite)
	{
		//center - subtract half sprite size to pos
		Vector2D centerPos((int)pos.getX() - (int)(mSprite.getWidth() / 2), (int)pos.getY() - (int)(mSprite.getHeight() / 2));
		GraphicsSystem::draw(centerPos, mSprite, facing);
	}
	else
	{
		GraphicsSystem::draw(pos, mSprite, facing);
	}

	if (gpGame->shouldShowDebugData())
	{
		Font* pFont = gpGame->getFontManager()->getFont(COUR_24_FONT_ID);
		assert(pFont);
		std::string desc = mpSteeringComponent->getSteeringDescription();
		GraphicsSystem::writeText(pos, *pFont, BLACK_COLOR, desc);
	}

	if (mShowTarget)
	{
		SteeringComponent* pSteering = getSteeringComponent();
		assert(pSteering != NULL);
		const Vector2D& targetLoc = pSteering->getTargetLoc();
		if (&targetLoc != &ZERO_VECTOR2D)
		{
			Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			assert(pTargetSprite != NULL);

			if (mCenterSprite)
			{
				//center - subtract half sprite size to targetLoc
				Vector2D centerPos((int)targetLoc.getX() - (int)(pTargetSprite->getWidth() / 2), (int)targetLoc.getY() - (int)(pTargetSprite->getHeight() / 2));
				GraphicsSystem::draw(centerPos, *pTargetSprite);
			}
			else
			{
				GraphicsSystem::draw(pos, *pTargetSprite);
			}
		}
	}

	mInventory.draw(pos);
}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
}

const Vector2D& Unit::getPosition() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);

	return pPosition->getPosition();
}

Vector2D Unit::getHeadingVector() const
{
	float facing = getFacing();
	return Vector2D::getVectorInDirection((double)facing);
}

void Unit::update(double elapsedTime)
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	PhysicsComponent* pPhysics = getPhysicsComponent();
	assert(pPhysics != NULL);
	if (pPhysics->getVelocity().hasNonZeroLength())
	{
		double facing = pPhysics->getVelocity().calcFacing();
		pPosition->setFacing((float)facing);
	}
	else //stopped
	{
	}
	if (mpBrain)
		mpBrain->update(elapsedTime);
}

PositionComponent* Unit::getPositionComponent() const
{
	return mpPositionComponent;
}

PhysicsComponent* Unit::getPhysicsComponent() const
{
	return mpPhysicsComponent;
}

SteeringComponent* Unit::getSteeringComponent() const
{
	return mpSteeringComponent;
}

void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}
}

bool Unit::addItem(Item* pItem)
{
	return mInventory.addItem(pItem);
}

bool Unit::hasRoomInInventory() const
{
	return mInventory.hasSlotsRemaining();
}

Item* Unit::getAndRemoveItem(ItemType type)
{
	return mInventory.getAndRemoveItem(type);
}

const Item* Unit::peekAtItem(ItemType type)
{
	return mInventory.peekAtItem(type);
}

bool Unit::hasItemOfType(ItemType type) const
{
	return mInventory.hasItemOfType(type);
}

void Unit::changeBrain(AIBrain::BrainType type)
{
	delete mpBrain;
	mpBrain = new AIBrain(this, type);
}

AIBrain::BrainType Unit::getBrainType() const
{
	if (mpBrain)
	{
		return mpBrain->getType();
	}
	else
	{
		return AIBrain::DEFAULT;
	}
}
