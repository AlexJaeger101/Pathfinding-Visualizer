#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>
#include <EventListener.h>

class System;
class GraphicsSystem;
class GraphicsBuffer;
class Font;
class FontManager;
class GraphicsBufferManager;
class SpriteManager;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class DataRepository;
class DataLoader;
class LocationManager;
class InputTranslator;
class GridVisualizer;
class Grid;
class GridGraph;
class GridPathfinder;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType SHACK_SPRITE_ID = 4;
const IDType FIELD_SPRITE_ID = 5;
const IDType SMITH_SPRITE_ID = 6;
const IDType MINE_SPRITE_ID = 7;
const IDType BANK_SPRITE_ID = 8;

const IDType COUR_48_FONT_ID = 0;
const IDType COUR_24_FONT_ID = 1;
const IDType COUR_6_FONT_ID = 2;
const IDType COUR_8_FONT_ID = 3;
const IDType COUR_10_FONT_ID = 4;
const IDType COUR_12_FONT_ID = 5;
const IDType COUR_14_FONT_ID = 6;

enum GameMode
{
	INVALID_GAME_MODE = -1,
	START_GAME_MODE,
	PLAY_GAME_MODE,
	PAUSE_GAME_MODE,
	EDIT_GAME_MODE,
	END_GAME_MODE
};

enum PathfindingType
{
	INVALID_PATHFINDING_TYPE = -1,
	BF_PATHFINDING,
	DF_PATHFINDING,
	DIJKSTRAS_PATHFINDING,
	ASTAR_PATHFINDING,

};

class Game:public EventListener
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void doLoop();

	inline System* getSystem() const { return mpSystem; };
	inline GraphicsSystem* getGraphicsSystem() const;
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline FontManager* getFontManager() const { return mpFontManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline DataRepository* getDataRepository() { return mpRepository; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline DataLoader* getLoader() const { return mpDataLoader; };
	inline LocationManager* getLocationManager() const { return mpLocationManager; };
	inline InputTranslator* getInputTranslator() const { return mpInputTranslator; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline GridPathfinder* getGridPathfinder() { return mpPathfinder; };

	inline GameMode getGameMode() const { return mCurrentMode; };
	inline bool shouldShowDebugData() const { return mDrawDebugData; };
private:
	System* mpSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	FontManager* mpFontManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	DataRepository* mpRepository;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	DataLoader* mpDataLoader;
	LocationManager* mpLocationManager = nullptr;
	InputTranslator* mpInputTranslator = nullptr;
	Grid* mpGrid = nullptr;
	GridVisualizer* mpGridVisualizer = nullptr;
	GridGraph* mpGridGraph = nullptr;
	GridPathfinder* mpPathfinder = nullptr;

	float mTimeLastFrame;
	float mTimeMult = 1.0f;
	bool mDrawDebugData = false;
	bool mShouldExit;
	UINT mTargetFPS=0;
	float mTargetElapsedTime = 0.0f;
	double mTotalGameTimeElapsed = 0.0;
	double mGameTimeAllotment = 0.0;
	GameMode mCurrentMode = INVALID_GAME_MODE;
	int mScore = 0;

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mShackLocationBufferID = "shack";
	GraphicsBufferID mFieldLocationBufferID = "field";
	GraphicsBufferID mSmithyLocationBufferID = "smith";
	GraphicsBufferID mMineLocationBufferID = "mine";
	GraphicsBufferID mBankLocationBufferID = "bank";

	void beginLoop();
	void processLoop();
	void endLoop();

	void render() const;
	void update(double dt);

	void loadFonts();
	void loadBuffers();
	void createSprites();
	void drawDebugData() const;
	void displayElapsedTime() const;
	void displayStartGame() const;
	void displayPauseGame() const;
	void displayEndGame() const;
	void saveGrid(std::ofstream& theStream);
	void loadGrid(std::ifstream& theStream);


	void setNewGameMode(GameMode newMode);
	void restartGame();

	void handleEvent(const Event& theEvent);
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

