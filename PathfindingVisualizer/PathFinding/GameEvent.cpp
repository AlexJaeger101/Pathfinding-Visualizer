#include "GameEvent.h"

MoveToEvent::MoveToEvent(const UnitID& unitID, const Vector2D& dest)
	:GameEvent(MOVE_TO)
	,mUnitID(unitID)
	,mDest(dest)
{
}

ChangeGameModeEvent::ChangeGameModeEvent(GameMode newMode)
	:GameEvent(CHANGE_GAME_MODE)
	,mNewMode(newMode)
{
}

ScorePointsEvent::ScorePointsEvent(int numPoints)
	:GameEvent(SCORE_POINTS)
	,mNumPoints(numPoints)
{
}

CreateEnemyEvent::CreateEnemyEvent(const Vector2D& pos)
	:GameEvent(CREATE_ENEMY)
	,mPos(pos)
{
}

ModTerrainEvent::ModTerrainEvent(const Vector2D& pixelLoc, int terrain, bool shouldAdd)
	:GameEvent(EDIT_MOD_TERRAIN)
	,mPixelLoc(pixelLoc)
	,mTerrain(terrain)
	,mAdd(shouldAdd)
{
}

LoadSaveMapEvent::LoadSaveMapEvent(bool shouldSave)
	:GameEvent(LOAD_SAVE_MAP)
	, mSave(shouldSave)
{
}

PathToEvent::PathToEvent(const Vector2D& location)
	:GameEvent(PATH_TO)
	,mPixelLoc(location)
{
}

ChangePathfindingEvent::ChangePathfindingEvent(const PathfindingType type)
	:GameEvent(CHANGE_PATHFINDING)
	,mType(type)
{
}
