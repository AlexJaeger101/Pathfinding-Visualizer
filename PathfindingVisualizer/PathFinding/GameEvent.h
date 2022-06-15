#pragma once

#include <Event.h>
#include <Vector2D.h>
#include "Location.h"
#include "Unit.h"
#include "Item.h"
#include "AIBrain.h"

enum GameEventType
{
	MOVE_TO = Event::NUM_EVENT_TYPES,
	CREATE_ENEMY,
	EXIT_GAME,
	CHANGE_GAME_MODE,
	SCORE_POINTS,
	EDIT_MOD_TERRAIN,
	LOAD_SAVE_MAP,
	PATH_TO,
	CHANGE_PATHFINDING,
	NUM_GAME_EVENT_TYPES
};

class GameEvent :public Event
{
public:
	GameEvent(GameEventType type) :Event((EventType)type) {};
	~GameEvent() {};
	GameEventType getGameEventType() const { return (GameEventType)getType(); };
};

class MoveToEvent : public GameEvent
{
public:
	MoveToEvent(const UnitID& unitID, const Vector2D& dest);
	~MoveToEvent() {};

	const UnitID& getUnitID() const { return mUnitID; };
	const Vector2D& getDestination() const { return mDest; };

private:
	UnitID mUnitID;
	Vector2D mDest;
};

class CreateEnemyEvent : public GameEvent
{
public:
	CreateEnemyEvent(const Vector2D& pos);
	~CreateEnemyEvent() {};

	const Vector2D& getPos() const { return mPos; };

private:
	Vector2D mPos;
};

class ChangeGameModeEvent : public GameEvent
{
public:
	ChangeGameModeEvent(GameMode newMode);
	~ChangeGameModeEvent() {};

	GameMode getNewMode() const { return mNewMode; };

private:
	GameMode mNewMode;
};

class ScorePointsEvent : public GameEvent
{
public:
	ScorePointsEvent(int numPoints);
	~ScorePointsEvent() {};

	int getNumPoints() const { return mNumPoints; };

private:
	int mNumPoints;
};

class ModTerrainEvent : public GameEvent
{
public:
	ModTerrainEvent(const Vector2D& pixelLoc, int terrain, bool shouldAdd);
	~ModTerrainEvent() {};

	const Vector2D& getPixelLocation() const { return mPixelLoc; };
	int getTerrain() const { return mTerrain; };
	bool shouldAdd() const { return mAdd; };

private:
	Vector2D mPixelLoc;
	int mTerrain;
	bool mAdd;
};

class LoadSaveMapEvent : public GameEvent
{
public:
	LoadSaveMapEvent(bool shouldSave);
	~LoadSaveMapEvent() {};

	bool shouldSave() const { return mSave; };

private:
	bool mSave;
};

class PathToEvent : public GameEvent
{
public:
	PathToEvent(const Vector2D& location);
	~PathToEvent() {};

	const Vector2D& getPixelLocation() const { return mPixelLoc; };

private:
	Vector2D mPixelLoc;
};

class ChangePathfindingEvent : public GameEvent
{
public:
	ChangePathfindingEvent(const PathfindingType type);
	~ChangePathfindingEvent() {};
	
	const PathfindingType getType() const { return mType; };

private:
	PathfindingType mType;
};

